// ------------------------------------------------------------------------------ //
// DISCLAIMER: We take no credit regarding this class							  //
// This is needed to make the Solar Light class, we use a bezier curve to		  //
// describe the path of the Solar Light											  //
// Credits go to user "yaaashua" who uploaded these files						  //
// https://discuss.cocos2d-x.org/t/beding-warping-sprites-by-bezier/33758/8		  //
// ------------------------------------------------------------------------------ //
#include "TexturedSpline.h"

USING_NS_CC;

TexturedSpline::TexturedSpline()
	: m_texture(nullptr)
	, m_subvisionLevel(0)
	, m_strokeScale(1)
	, m_isTextureRepeated(true)
	, m_interpolationType(InterpolationType::CATMULL_ROM)
{
}

TexturedSpline::~TexturedSpline()
{
	CC_SAFE_RELEASE_NULL(m_texture);
}

TexturedSpline* TexturedSpline::create(std::vector<cocos2d::Vec2> path, unsigned int subvisionLevel, const std::string& imagePath, InterpolationType type)
{
	auto instance = new (std::nothrow) TexturedSpline();
	if(instance && instance->init( path, subvisionLevel, imagePath, type))
	{
		return instance;
	}
	CC_SAFE_DELETE(instance);
	return nullptr;
}

TexturedSpline* TexturedSpline::create(std::vector<cocos2d::Vec2> path, unsigned int subvisionLevel, Texture2D* texture, InterpolationType type)
{
	auto instance = new (std::nothrow) TexturedSpline();
	if(instance && instance->initWithTexture( path, subvisionLevel, texture, type))
	{
		return instance;
	}
	CC_SAFE_DELETE(instance);
	return nullptr;
}

bool TexturedSpline::init(std::vector<cocos2d::Vec2> path, unsigned int subvisionLevel, const std::string& imagePath, InterpolationType type)
{
	auto textureCache = Director::getInstance()->getTextureCache();
	auto texture = textureCache->addImage(imagePath);
	return initWithTexture(path, subvisionLevel, texture, type);
}

bool TexturedSpline::initWithTexture(std::vector<cocos2d::Vec2> path, unsigned int subvisionLevel, Texture2D* texture, InterpolationType type)
{
	m_interpolationType = type;
	if(texture == nullptr)
	{
		CCLOGERROR("[TexturedSpline] Invalid texture");
		return false;
	}
	int h = texture->getContentSizeInPixels().height;
	int w = texture->getContentSizeInPixels().width;

	if((h > 0 && ((h & (h - 1)) == 0) == false) || (h > 0 && ((h & (h - 1)) == 0)) == false)
	{
		const char* errorMsg = "[TexturedSpline] Invalid texture: WIDTH AND HEIGHT must be a power of 2";

		CCLOGERROR(errorMsg);
		assert(false && errorMsg);
		return false;
	}

	if(path.size() < 4)
	{
		const char* errorMsg = "[TexturedSpline] Invalid path. It contains less than 4 points. Required minimum: 4";

		CCLOGERROR(errorMsg);
		assert(false && errorMsg);
		return false;
	}
	this->path = path;

	setAnchorPoint(Vec2(0.5f, 0.5f));
	setTexture(texture);
	if(buildGeometry(path, subvisionLevel) == false)
	{
		return false;
	}

	// configure texture rendering
	if(!texture || !texture->hasPremultipliedAlpha())
	{
		m_blendFunc = BlendFunc::ALPHA_NON_PREMULTIPLIED;
		setOpacityModifyRGB(false);
	}
	else
	{
		m_blendFunc = BlendFunc::ALPHA_PREMULTIPLIED;
		setOpacityModifyRGB(true);
	}

	//set default shader
	setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE));

	return true;
}

bool TexturedSpline::buildGeometry(std::vector<cocos2d::Vec2> path, unsigned int subvisionLevel)
{
	if(path.size() < 2)
	{
		CCLOGERROR("[TexturedSpline] Invalid path: path has %f elements. Requires min 4", path.size());
		return false;
	}

	auto textureSize = m_texture->getContentSize() * m_strokeScale;

	//calculate radii
	auto strokeHalfWidth = textureSize.height * 0.5f;
	m_subvisionLevel = subvisionLevel;
	

	auto GetNormalVec = [](unsigned int pointNum, const std::vector<cocos2d::Vec2>& points)
	{
		assert(pointNum < points.size());
		assert(points.size() > 1);

		bool onlyLeft = false;
		bool onlyRight = pointNum == 0;

		if(pointNum == points.size() - 1)
		{
			//if its the last point, return only the last (right) segment
			onlyLeft = true;
			pointNum--;
		}

		cocos2d::Vec2 perpendicularL, perpendicularR;
		if(!onlyLeft)	//right segment
		{
			cocos2d::Vec2 dir = points[pointNum + 1] - points[pointNum];	
			perpendicularR.set(-dir.y, dir.x);
			perpendicularR.normalize();
		}

		if(!onlyRight)	//left segment
		{
			cocos2d::Vec2 dir = points[pointNum] - points[pointNum - 1];
			perpendicularL.set(-dir.y, dir.x);
			perpendicularL.normalize();
		}

		if(onlyLeft)	return perpendicularL;
		if(onlyRight)	return perpendicularR;

		cocos2d::Vec2 mid = perpendicularL + perpendicularR;
		mid.normalize();
		return mid;
	};

	std::vector<cocos2d::Vec2> interpolated = GetInterpolatedPath(subvisionLevel);

	//build buffers interleaved between top and bottom parallel lines
	size_t vertCount = interpolated.size() * 2;
	m_vertices.resize(vertCount);
	m_texCoords.resize(vertCount);

	//get top and bot points
	std::vector<cocos2d::Vec2> top, bot;
	unsigned int i = 0;
	for(cocos2d::Vec2 v : interpolated)
	{
		cocos2d::Vec2 normalTop = GetNormalVec(i++, interpolated);
		top.push_back(v + (normalTop * textureSize.height / 2));
		bot.push_back(v - (normalTop * textureSize.height / 2));
	}

	//get % uv position
	std::vector<float> topDist;
	float totalDistTop = 0;
	topDist.push_back(0);
	for(i = 0; i < interpolated.size() - 1; i++)
	{
		topDist.push_back(top[i].getDistance(top[i + 1]));
		totalDistTop += topDist.back();
	}

	//calulate best fitting texture repeat count
	auto textureRepeatCount = 1;
	if(m_isTextureRepeated)
	{
		textureRepeatCount = MAX(1.0f, roundf((totalDistTop / textureSize.width)));
	}

	//mapping uv
	float u = 0;
	for(i = 0; i < interpolated.size(); i++)
	{
		size_t idx = i * 2;

		u += topDist[i] / totalDistTop;	//increase

		//top
		m_texCoords[idx].u = u * textureRepeatCount;
		m_texCoords[idx].v = 0;
		m_vertices[idx].x = top[i].x;
		m_vertices[idx].y = top[i].y;

		//bot
		m_texCoords[idx + 1].u = u* textureRepeatCount;
		m_texCoords[idx + 1].v = 1;
		m_vertices[idx + 1].x = bot[i].x;
		m_vertices[idx + 1].y = bot[i].y;
	}

	//set contentSize
	float minX, maxX, minY, maxY;
	minX = maxX = top[0].x;
	minY = maxY = top[0].y;
	for(i = 0; i < interpolated.size(); i++)
	{
		if(top[i].x < minX)	minX = top[i].x;
		if(top[i].x > maxX)	maxX = top[i].x;
		if(top[i].y < minY)	minY = top[i].y;
		if(top[i].y > maxY)	maxX = top[i].y;
		if(bot[i].x < minX)	minX = bot[i].x;
		if(bot[i].x > maxX)	maxX = bot[i].x;
		if(bot[i].y < minY)	minY = bot[i].y;
		if(bot[i].y > maxY)	maxX = bot[i].y;
	}

	setContentSize(Size(maxX - minX, maxY - minY));
	return true;
}

Texture2D* TexturedSpline::getTexture() const
{
	return m_texture;
}

void TexturedSpline::setTexture(Texture2D *texture)
{
	CC_SAFE_RELEASE_NULL(m_texture);
	if(texture != nullptr)
	{
		if(m_isTextureRepeated)
		{
			Texture2D::TexParams textureParams;
			textureParams.minFilter = GL_LINEAR;
			textureParams.magFilter = GL_LINEAR;
			textureParams.wrapS = GL_REPEAT; //default to horizontal wrapping
			textureParams.wrapT = GL_CLAMP_TO_EDGE;
			texture->setTexParameters(textureParams);
		}
		m_texture = texture;
		m_texture->retain();

		//rebuild the geometry to match new texture
		if(path.size() > 4 && m_subvisionLevel > 0)
		{
			buildGeometry(path, m_subvisionLevel);
		}
	}
}

void TexturedSpline::setBlendFunc(const BlendFunc &blendFunc)
{
	m_blendFunc = blendFunc;
}

const BlendFunc& TexturedSpline::getBlendFunc() const
{
	return m_blendFunc;
}

void TexturedSpline::setStrokeScale(const float &scale)
{
	if(scale != m_strokeScale)
	{
		m_strokeScale = scale;
		buildGeometry(path, m_subvisionLevel);
	}
}

const float& TexturedSpline::getStrokeScale() const
{
	return m_isTextureRepeated;
}

void TexturedSpline::setIsTextureRepeated(const bool &isTextureRepeated)
{
	if(isTextureRepeated != m_isTextureRepeated)
	{
		m_isTextureRepeated = isTextureRepeated;
		setTexture(m_texture);
	}
}

const bool& TexturedSpline::getIsTextureRepeated() const
{
	return m_isTextureRepeated;
}

std::vector<cocos2d::Vec2> TexturedSpline::GetInterpolatedPath(unsigned int segments) const
{
	if(segments < 1)
	{
		segments = 1;
	}

	std::vector<cocos2d::Vec2> newPath;

	// # of segments = #of points - 1
	for(unsigned int pointNum = 0; pointNum < path.size() - 1; pointNum++)
	{
		for(unsigned int s = 0; s < segments; s++)
		{
			float t = s / (float)segments;
			newPath.push_back(GetInterpolation(t, pointNum));
		}
	}
	newPath.push_back(path.back());
	return newPath;
}

cocos2d::Vec2 TexturedSpline::GetInterpolation(float t, unsigned int pointNum) const
{
	unsigned int sz = path.size();
	assert(t >= 0.0f && t <= 1.0f);
	assert(sz >= 4 && pointNum < sz - 1);

	if(pointNum == sz - 1)
	{
		return path[sz - 1];
	}
	else if(pointNum == 0)
	{
		if(t == 0)
		{
			return path[0];
		}
		return GetInterpolation(m_interpolationType, t, path[0], path[0], path[1], path[2]);
	}
	else if(pointNum == sz - 2)
	{
		return GetInterpolation(m_interpolationType, t, path[sz - 3], path[sz - 2], path[sz - 1], path[sz - 1]);
	}

	return GetInterpolation(m_interpolationType, t, path[pointNum - 1], path[pointNum], path[pointNum + 1], path[pointNum + 2]);
	}

cocos2d::Vec2 TexturedSpline::GetInterpolation(InterpolationType type, float t, cocos2d::Vec2 p0, cocos2d::Vec2 p1, cocos2d::Vec2 p2, cocos2d::Vec2 p3)
{
	// I got the formulas from here: https://zone.ni.com/reference/en-XX/help/371093M-01/nimclvfb/controlip.splineinterpolation_00b2001b/
	switch(type)
	{
		default:
		case TexturedSpline::InterpolationType::CATMULL_ROM:
			return 0.5f *
				((2 * p1) +
				(p2 - p0) * t +
					(2 * p0 - 5 * p1 + 4 * p2 - p3) * t * t +
					(3 * p1 - p0 - 3 * p2 + p3) * t * t * t);

		case TexturedSpline::InterpolationType::B_CUBIC:
			return (1.0f / 6.0f) *
				((p0 + 4 * p1 + p2) +
				(3 * p2 - 3 * p0) * t +
					(3 * p0 - 6 * p1 + 3 * p2) *t*t +
					(3 * p1 - p0 - 3 * p2 + p3) * t * t * t);
		case TexturedSpline::InterpolationType::LINEAR:
			return
				(p2 - p1) * t +
				(p1);
	}
}

void TexturedSpline::onDraw(const Mat4 &transform, uint32_t flags)
{
	if(m_texture == nullptr || m_vertices.empty())
	{
		return;
	}

	getGLProgram()->use();
	getGLProgram()->setUniformsForBuiltins(transform);

	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORD);
	GL::blendFunc(m_blendFunc.src, m_blendFunc.dst);

	GL::bindTexture2D(m_texture->getName());

#ifdef EMSCRIPTEN
	setGLBufferData(m_vertices, (sizeof(Vec2) * m_vertices.size()), 0);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, 0);

	setGLBufferData(_texCoords, (sizeof(Tex2F) * m_texCoords.size()), 1);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, 0);
#else
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, m_vertices.data());
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, m_texCoords.data());
#endif // EMSCRIPTEN

	glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)m_vertices.size());
	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, m_vertices.size());
}

void TexturedSpline::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	_customCommand.init(_globalZOrder, transform, flags);
	_customCommand.func = CC_CALLBACK_0(TexturedSpline::onDraw, this, transform, flags);
	renderer->addCommand(&_customCommand);
}

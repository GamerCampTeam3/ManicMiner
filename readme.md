# LICENSE for this project #
Copyright 2020 Alex Darby / Gamer Camp (BCU) Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

# overview #
This is a framework for making 2D games *based on* cocos2d-x which is used for Modules 1 and 2 of Gamer Camp

- The Module 1 version is on branch "Module_1_Version"
- The Module 2 version is on branch "Module_2_Version"

The main difference between the two is that Module 2 uses OGMO editor (https://ogmo-editor-3.github.io/, https://github.com/ogmo-editor-3) to load levels and uses a factory pattern to create level objects from the leel.

# required tools #
This framework relies on two paid for tools:
TexturePacker and PhysicsEditor which are available from here: https://www.codeandweb.com/store

Texture Packer has a 7 day free trial for the pro features needed for the framework's data toolchain, beyond this you will have to pay for Texture packer or export manually using the limited free features.
Physics Editor has a free limited feature version which you can use.

# relevant links #
Content of (/SystemDiagrams): 

- hand-drawn overview of cocos' system  (SystemDiagrams/CocosArchitectureOverview.jpg)
- hand-drawn overview of the way in which the GCFramework works with Cocos (SystemDiagrams/GCTemplateClassDiagram.jpg)
- hand-drawn overview of cocos' "memory management" paradigm (SystemDiagrams/CocosMemoryManagement.jpg)
- *new* improved hand-drawn overview of main class relations in GCFramework & how it relates to cocos and box2d (SystemDiagrams/GCFrameworkClassRelationsOverview.jpg)

Content of (GamerCampDocs/)

- explanation of the way art assets get into the game (GamerCampDocs/ArtPipeline.pdf)
- Gamer Camp coding standards (fwiw!) (GamerCampDocs/CodingStandards.pdf)

Otherwise feel free to google for cocos2d-x	and read their overview stuff, but bear in mind that the GCFramework really only uses Cocos as a renderer...

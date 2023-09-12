#pragma once

#ifndef POST_PROCESSOR_H
#define POST_PROCESSOR_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "sprite_renderer.h"
#include "shader.h"


// PostProcessor hosts all PostProcessing effects for the Breakout
// Game. It renders the game on a textured quad after which one can
// enable specific effects by enabling either the Confuse, Chaos or 
// Shake boolean. 
// It is required to call BeginRender() before rendering the game
// and EndRender() after rendering the game for the class to work.
class PostProcessor
{
public:
	// state
	Shader	  PostProcessingShader;
	Texture2D Texture;
	unsigned int Width, Height;
	bool Confuse, Chaos, Shake; // options
	PostProcessor(Shader shader, unsigned int width, unsigned int height);
	void BeginRender();//prepares the postprocessor's framebuffer operations before rendering the game
	void EndRender();//should be called after rendering the game, so it stores all the rendered data into a texture object
	void Render(float time); //renders the PostProcessor texture quad ( as a screen-encompassing large sprite)
private:
	//render state
	unsigned int MSFBO, FBO; // MSFBO = Multisampled FBO. FBO is regular, used for blitting MS color-buffer to texture
	unsigned int RBO; // RBO is used for multisampled color buffer;
	unsigned int VAO;
	void initRenderData(); //initialize quad for rendering postprocessing texture
};

#endif


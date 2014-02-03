/* Wanderlust code is styled as follows:

objectnames are in all lower case, while VariableNames are UpperCamelCase. Exceptions are
temporary variables and which can be capitalized however.
Rationale: You don't write "I'd love to visit the City (type) of paris (instance)". You write "city of Paris."
This is probably stupid and a better programmer will eventually call me on it, but eh.

Braces are()
{
	//like this.
	//What's the term, again?
	return Braces("I haven't got a clue.");
}

Hungarian notation is used but only in a few specific cases. things like int iPositionX are not used.
Specifically, member variables are prefixed with a lowercase m - mJabberwok, for example. Pointers are
either prefixed with a lowercase p - one for each level of indirection, i.e. GameEntity** ppPrecachedEnts;
or suffixed with Ptr or something similar (again, once for each level of indirection).

Member functions are UpperCamel, with the exception of getters and setters, which are lowerCamelCase -
so you'll end up with something like obj.setPositionX(10); which works on a variable called (thanks
to our use of UpperCamelCase for variable names) mPositionX.

Use typename, not class, for templates.

Use the goto statement as much as possible. That's how the pros do it. B) B) B)B)B)BB)))))
*/


#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <gl/glew.h>
#include <gl/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SFML/OpenGL.hpp>
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <iomanip>
#include "..\game\world\chunk.h"
#include "..\game\world\space.h"
#include "..\render\block\chunkblockmodel.h"
//#include "..\game\world\chunkcoordstofilename.h"
#include "..\render\resourcemanagement\glblocktextureindex.h"
#include "..\render\block\blockart.h"
#include "..\game\block\blockindex.h"

#include "misc\intstrbase64.h"
#include "math\distance3d.h"
#include "math\typedefs.h"

#include <SFML\Graphics.hpp>

using namespace Wanderlust;

// vvvvvvvv DELETE THIS BEFORE RELEASE
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path)
{
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}



	GLint Result = GL_FALSE;
	int InfoLogLength;



	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> VertexShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);




	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);




	// Link the program
	fprintf(stdout, "Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> ProgramErrorMessage( max(InfoLogLength, int(1)) );
	glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
	fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);


	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
};
// ^^^^^^^^ DELETE THIS BEFORE RELEASE

int main()
{
	cout << Base64ToInt(IntToBase64(262154)) << endl;
	cout << Distance3D(vector3(1,1,1), vector3(0,0,0)) << endl;
    std::ifstream fin("test.yaml");
	YAML::Parser parser(fin);
	YAML::Node doc;
	parser.GetNextDocument(doc);
	int ScreenWidth = 800, ScreenHeight = 600;
	*(doc.FindValue("window")->FindValue("width")) >> ScreenWidth;
	*(doc.FindValue("window")->FindValue("height")) >> ScreenHeight;

	
    sf::Window window(sf::VideoMode(ScreenWidth, ScreenHeight), "OpenGL", sf::Style::Default, sf::ContextSettings(32));
    window.setVerticalSyncEnabled(true);
    bool running = true;

	window.setMouseCursorVisible(false);

	glewInit(); //MUST COME AFTER WINDOW INIT;

	space TestSpace;
	
    sf::Event event;

	glClearColor(0.0f, 0.0f, 0.3f, 0.0f);

	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_LESS); 

	glEnable(GL_CULL_FACE);
	
	glm::vec3 CameraPos = glm::vec3( 5, 0, 0 ); 
	real FieldOfView = 70.0f;
	
	real MouseSensitivity = 0.009f;
	real MoveSpeed = 5.0f;
	real HorizontalAngle = 0;
	real VerticalAngle = 0;

	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;

	// vvvvv FUCKING DELETE THIS IF YOU WANT TO LIVE
	GLuint programID = LoadShaders( "TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader" );
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	// ^^^^^^^^^^^

	sf::Clock Clock;
	sf::Time ElapsedTime;
	int MousePrevX = sf::Mouse::getPosition().x;
	int MousePrevY = sf::Mouse::getPosition().y;

	glm::mat4 ModelMatrix = glm::mat4(1.0);
	
	gl_blocktextureindex TestBlockTextures;
	TestBlockTextures.setTextureSize(64,64);
	
	char* ImgFileBuffer = 0;
	ifstream imgtest ("Testtex.png", ios::in|ios::binary|ios::ate);
	ifstream::pos_type size;
	if (imgtest.is_open())
	{
		size = imgtest.tellg();
		ImgFileBuffer = new char [size];
		imgtest.seekg (0, ios::beg);
		imgtest.read (ImgFileBuffer, size);
	}
	imgtest.close();
	TestBlockTextures.addTexture("test1", static_cast<void*>(ImgFileBuffer), size);
	delete [] ImgFileBuffer;
	
	imgtest.open("Testtex2.png", ios::in|ios::binary|ios::ate);
	if (imgtest.is_open())
	{
		size = imgtest.tellg();
		ImgFileBuffer = new char [size];
		imgtest.seekg (0, ios::beg);
		imgtest.read (ImgFileBuffer, size);
	}
	imgtest.close();
	TestBlockTextures.addTexture("test2", static_cast<void*>(ImgFileBuffer), size);
	delete [] ImgFileBuffer;

	imgtest.open("teststone.png", ios::in|ios::binary|ios::ate);
	if (imgtest.is_open())
	{
		size = imgtest.tellg();
		ImgFileBuffer = new char [size];
		imgtest.seekg (0, ios::beg);
		imgtest.read (ImgFileBuffer, size);
	}
	imgtest.close();
	TestBlockTextures.addTexture("Stone", static_cast<void*>(ImgFileBuffer), size);
	delete [] ImgFileBuffer;

	imgtest.open("testdirt.png", ios::in|ios::binary|ios::ate);
	if (imgtest.is_open())
	{
		size = imgtest.tellg();
		ImgFileBuffer = new char [size];
		imgtest.seekg (0, ios::beg);
		imgtest.read (ImgFileBuffer, size);
	}
	imgtest.close();
	TestBlockTextures.addTexture("Dirt", static_cast<void*>(ImgFileBuffer), size);
	delete [] ImgFileBuffer;
	
	imgtest.open("testgrass.png", ios::in|ios::binary|ios::ate);
	if (imgtest.is_open())
	{
		size = imgtest.tellg();
		ImgFileBuffer = new char [size];
		imgtest.seekg (0, ios::beg);
		imgtest.read (ImgFileBuffer, size);
	}
	imgtest.close();
	TestBlockTextures.addTexture("Grass", static_cast<void*>(ImgFileBuffer), size);
	delete [] ImgFileBuffer;

	imgtest.open("testglass.png", ios::in|ios::binary|ios::ate);
	if (imgtest.is_open())
	{
		size = imgtest.tellg();
		ImgFileBuffer = new char [size];
		imgtest.seekg (0, ios::beg);
		imgtest.read (ImgFileBuffer, size);
	}
	imgtest.close();
	TestBlockTextures.addTexture("Glass", static_cast<void*>(ImgFileBuffer), size);
	delete [] ImgFileBuffer;

	std::cout << TestBlockTextures.getTextureCount() << std::endl;
	std::cout << TestBlockTextures.getTextureIDByName("test1") << std::endl;
	TestBlockTextures.Build();
	glBindTexture(GL_TEXTURE_2D_ARRAY, TestBlockTextures.getTextureArrayID());
	
	glUseProgram(programID);



	
	blockart TestArt1;
	for(int i = 0; i < 6; i++)
	{
		TestArt1.setTexture(i,TestBlockTextures.getTextureIDByName("test1")); 
	}
	blockart TestArt2;
	for(int i = 0; i < 6; i++)
	{
		TestArt2.setTexture(i,TestBlockTextures.getTextureIDByName("test2")); 
	}
	blockart TestArtStone;
	for(int i = 0; i < 6; i++)
	{
		TestArtStone.setTexture(i,TestBlockTextures.getTextureIDByName("Stone")); 
	}
	blockart TestArtDirt;
	for(int i = 0; i < 6; i++)
	{
		TestArtDirt.setTexture(i,TestBlockTextures.getTextureIDByName("Dirt")); 
	}
	blockart TestArtGrass;
	for(int i = 0; i < 6; i++)
	{
		TestArtGrass.setTexture(i,TestBlockTextures.getTextureIDByName("Grass")); 
	}
	blockart TestArtGlass;
	for(int i = 0; i < 6; i++)
	{
		TestArtGlass.setTexture(i,TestBlockTextures.getTextureIDByName("Glass"));
		TestArtGlass.setCullsOthers(NumToAxis[i], false);
	}



	blockindex TestBlockIndex;
	TestBlockIndex.addBlock("air");
	TestBlockIndex.getBlockByID(TestBlockIndex.addBlock("test1"))->setBlockArt(&TestArt1);
	TestBlockIndex.getBlockByID(TestBlockIndex.addBlock("test2"))->setBlockArt(&TestArt2);
	TestBlockIndex.getBlockByID(TestBlockIndex.addBlock("Stone"))->setBlockArt(&TestArtStone);
	TestBlockIndex.getBlockByID(TestBlockIndex.addBlock("Dirt"))->setBlockArt(&TestArtDirt);
	TestBlockIndex.getBlockByID(TestBlockIndex.addBlock("Grass"))->setBlockArt(&TestArtGrass);
	TestBlockIndex.getBlockByID(TestBlockIndex.addBlock("Glass"))->setBlockArt(&TestArtGlass);

	
	
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_BLEND);
	glAlphaFunc(GL_GREATER, 0.5);
	glEnable(GL_ALPHA_TEST);

	chunkgenerator TestGen;
	TestGen.regBlockIndex(&TestBlockIndex);

	TestSpace.regChunkGen(&TestGen);

	/*int Gensize = 4;

	for(int XIter = -(Gensize/2); XIter < (Gensize/2); XIter++)
	{
		for(int ZIter = -(Gensize/2); ZIter < (Gensize/2); ZIter++)
		{
			TestSpace.genColumn(XIter,ZIter);
		}
	}*/

	TestSpace.genChunk(0,0,0);



	
	chunkblockmodel TestModel;
	
	TestModel.regBlockIndex(&TestBlockIndex);
	TestModel.regChunk(TestSpace.getChunk(0, 0, 0));
	TestModel.Rebuild();

	int CurrentBrush = 0;

	int PreviousWheel;

    while (running)
    {
		ElapsedTime = Clock.restart();
		
		HorizontalAngle += ((ScreenWidth/2)-sf::Mouse::getPosition(window).x) * MouseSensitivity;
		VerticalAngle += ((ScreenHeight/2)-sf::Mouse::getPosition(window).y) * MouseSensitivity;
		MousePrevX = sf::Mouse::getPosition().x;
		MousePrevY = sf::Mouse::getPosition().y;
		sf::Mouse::setPosition(sf::Vector2i(ScreenWidth/2, ScreenHeight/2), window);

		
		if(VerticalAngle > 1.57)
		{
			VerticalAngle = 1.57;
		}
		else if(VerticalAngle < -1.57)
		{
			VerticalAngle = -1.57;
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			running = false;
		}
		
		// vvvvvvvv DELETE THIS BEFORE RELEASE
		glm::vec3 direction(
			cos(VerticalAngle) * sin(HorizontalAngle), 
			sin(VerticalAngle),
			cos(VerticalAngle) * cos(HorizontalAngle)
		);
		glm::vec3 right = glm::vec3(
			sin(HorizontalAngle - 3.14f/2.0f), 
			0,
			cos(HorizontalAngle - 3.14f/2.0f)
		);
		glm::vec3 up = glm::cross( right, direction );
		// ^^^^^^^
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			CameraPos -= (ElapsedTime.asSeconds() * MoveSpeed) * right;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			CameraPos += (ElapsedTime.asSeconds() * MoveSpeed) * right;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			CameraPos += (ElapsedTime.asSeconds() * MoveSpeed) * direction;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			CameraPos -= (ElapsedTime.asSeconds() * MoveSpeed) * direction;
		}
		
        // handle events
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                // end the program
                running = false;
            }
            else if (event.type == sf::Event::Resized)
            {
                // adjust the viewport when the window is resized
                glViewport(0, 0, event.size.width, event.size.height);
            }
			else if (event.type == sf::Event::KeyReleased)
			{
				if(event.key.code == sf::Keyboard::F2)
				{
					char* ChunkFileBuffer = TestSpace.getChunk(0, 0, 0)->Save();
					ofstream Save ("test.wlc", ios::out|ios::binary|ios::trunc);
					ofstream::pos_type size = TestSpace.getChunk(0, 0, 0)->SaveSize();
					if (Save.is_open())
					{
						Save.write(ChunkFileBuffer, size);
					}
					Save.close();
					TestSpace.getChunk(0, 0, 0)->DoneSaving();					
				}
				else if(event.key.code == sf::Keyboard::F3)
				{
					real StartTime = Clock.getElapsedTime().asSeconds();
					char* ChunkFileBuffer = 0;
					ifstream Load ("test.wlc", ios::in|ios::binary|ios::ate);
					ifstream::pos_type size;
					if (Load.is_open())
					{
						size = Load.tellg();
						ChunkFileBuffer = new char [size];
						Load.seekg (0, ios::beg);
						Load.read (ChunkFileBuffer, size);
					}
					Load.close();
					TestSpace.getChunk(0, 0, 0)->Load(ChunkFileBuffer, size);
					TestSpace.getChunk(0, 0, 0)->DoneLoading();
					delete [] ChunkFileBuffer;
					TestModel.Rebuild();
					cout << "Load & rebuild time: " << Clock.getElapsedTime().asSeconds() - StartTime << std::endl;
				}
				else if(event.key.code == sf::Keyboard::F4)
				{
					TestGen.GenerateChunk(TestSpace.getChunk(0, 0, 0));
					TestModel.Rebuild();
				}
				else if(event.key.code == sf::Keyboard::Escape)
				{
					running = false;
				}
			}
			else if (event.type == sf::Event::MouseWheelMoved)
			{
				CurrentBrush += event.mouseWheel.delta;
				if(CurrentBrush >= TestBlockIndex.getBlockCount())
				{
					CurrentBrush = 0;
				}
				else if(CurrentBrush < 0)
				{
					CurrentBrush = TestBlockIndex.getBlockCount() - 1;
				}
				cout << TestBlockIndex.getNameByBlockID(CurrentBrush) << std::endl;
			}
			else if (event.type == sf::Event::MouseButtonReleased)
			{
				int tempx, tempy, tempz;
				if (direction.x > 0)
				{
					if(abs(direction.x) >= 0.5)
					{
						tempx = (int)ceil(direction.x);
					}
					else
					{
						tempx = (int)floor(direction.x);
					}
				}
				else
				{
					if(abs(direction.x) >= 0.5)
					{
						tempx = (int)floor(direction.x);
					}
					else
					{
						tempx = (int)ceil(direction.x);
					}
				}
				if (direction.y > 0)
				{
					if(abs(direction.y) >= 0.5)
					{
						tempy = (int)ceil(direction.y);
					}
					else
					{
						tempy = (int)floor(direction.y);
					}
				}
				else
				{
					if(abs(direction.y) >= 0.5)
					{
						tempy = (int)floor(direction.y);
					}
					else
					{
						tempy = (int)ceil(direction.y);
					}
				}
				if (direction.z > 0)
				{
					if(abs(direction.z) >= 0.5)
					{
						tempz = (int)ceil(direction.z);
					}
					else
					{
						tempz = (int)floor(direction.z);
					}
				}
				else
				{
					if(abs(direction.z) >= 0.5)
					{
						tempz = (int)floor(direction.z);
					}
					else
					{
						tempz = (int)ceil(direction.z);
					}
				}
				if(TestSpace.getBlockExists((CameraPos.x+tempx), (CameraPos.y+tempy), (CameraPos.z+tempz)))
				{
					if(event.mouseButton.button == sf::Mouse::Button::Left)
					{
						if(TestSpace.getBlock((CameraPos.x+tempx), (CameraPos.y+tempy), (CameraPos.z+tempz)) != 0)
						{
							real StartTime = Clock.getElapsedTime().asSeconds();
							TestSpace.setBlock((CameraPos.x+tempx), (CameraPos.y+tempy), (CameraPos.z+tempz), 0);
							TestModel.Rebuild();
							cout << "Rebuild time: " << Clock.getElapsedTime().asSeconds() - StartTime << std::endl;
						}
					}
					if(event.mouseButton.button == sf::Mouse::Button::Right)
					{
						if(TestSpace.getBlock((CameraPos.x+tempx), (CameraPos.y+tempy), (CameraPos.z+tempz)) == 0)
						{
							real StartTime = Clock.getElapsedTime().asSeconds();
							TestSpace.setBlock((CameraPos.x+tempx), (CameraPos.y+tempy), (CameraPos.z+tempz), CurrentBrush);
							TestModel.Rebuild();
							cout << "Rebuild time: " << Clock.getElapsedTime().asSeconds() - StartTime << std::endl;
						}
					}
					if(event.mouseButton.button == sf::Mouse::Button::Middle)
					{
						CurrentBrush = TestSpace.getBlock((CameraPos.x+tempx), (CameraPos.y+tempy), (CameraPos.z+tempz));
						cout << TestBlockIndex.getNameByBlockID(CurrentBrush) << std::endl;
					}
				}
			}
        }
        // clear the buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// vvvvvvv
		ProjectionMatrix = glm::perspective(FieldOfView, 4.0f / 3.0f, 0.1f, 1000.0f);
		// Camera matrix
		ViewMatrix       = glm::lookAt(
									CameraPos,           // Camera is here
									CameraPos+direction, // and looks here : at the same position, plus "direction"
									up                  // Head is up (set to 0,-1,0 to look upside-down)
							   );
		//glEnableVertexAttribArray(0);
     //   glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
       /* glVertexAttribPointer(
           0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
           3,                  // size
           GL_real,           // type
           GL_FALSE,           // normalized?
           0,                  // stride
           (void*)0            // array buffer offset
        );*/
		/*glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size : U+V => 2
			GL_real,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);*/


				// Use our shader
		glUseProgram(programID);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D_ARRAY, TestBlockTextures.getTextureArrayID());
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		//glUniform1i(TestBlockTextures.getTextureArrayID(), 0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		// ^^^^^^^^ DELETE THIS BEFORE RELEASE
		TestModel.Draw();
		//ModelMatrix = glm::mat4(1.0);

        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }

    // release resources...
	
    return 0;
}

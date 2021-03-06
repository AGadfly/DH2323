/*
#include <iostream>
#include <glm/glm.hpp>
#include <SDL.h>
#include "SDLauxiliary.h"
#include "TestModel.h"

using namespace std;
using glm::vec3;
using glm::mat3;

// ----------------------------------------------------------------------------
// GLOBAL VARIABLES

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;
SDL_Surface* screen;
int t;

// ----------------------------------------------------------------------------
// FUNCTIONS

void Update();
void Draw();

int main(int argc, char* argv[])
{
	screen = InitializeSDL(SCREEN_WIDTH, SCREEN_HEIGHT);
	t = SDL_GetTicks();	// Set start value for timer.

	while (NoQuitMessageSDL())
	{
		Update();
		Draw();
	}

	SDL_SaveBMP(screen, "screenshot.bmp");
	return 0;
}

void Update()
{
	// Compute frame time:
	int t2 = SDL_GetTicks();
	float dt = float(t2 - t);
	t = t2;
	cout << "Render time: " << dt << " ms." << endl;
}

void Draw()
{
	if (SDL_MUSTLOCK(screen))
		SDL_LockSurface(screen);

	for (int y = 0; y<SCREEN_HEIGHT; ++y)
	{
		for (int x = 0; x<SCREEN_WIDTH; ++x)
		{
			vec3 color(1, 0.5, 0.5);
			PutPixelSDL(screen, x, y, color);
		}
	}

	if (SDL_MUSTLOCK(screen))
		SDL_UnlockSurface(screen);

	SDL_UpdateRect(screen, 0, 0, 0, 0);
}


*/
#include <iostream>
#include <glm/glm.hpp>
#include <SDL.h>
#include "SDLauxiliary.h"
#include "TestModel.h"
#include <math.h>  

using namespace std;
using glm::vec3;
using glm::mat3;

// ----------------------------------------------------------------------------
// GLOBAL VARIABLES

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;
SDL_Surface* screen;
int t;
float m = std::numeric_limits<float>::max();
float focalLength = SCREEN_HEIGHT / 2;
vec3 cameraPos(0, 0, 0);
std::vector<Triangle>& triangles;

//////////////////////////////////////////////
//                STRUCTURE                 //
//////////////////////////////////////////////
struct Intersection
{
	vec3 position;
	float distance;
	int triangleIndex;
};


// ----------------------------------------------------------------------------
// FUNCTIONS

void Update();
void Draw();
bool ClosestIntersection(
	vec3 start,
	vec3 dir,
	const vector<Triangle>& triangles,
	Intersection& closestIntersection
	);
vec3 unit_vector(vec3 a, vec3 b);

//////////////////////////////////////////////
//              MAIN FUNCTION               //
//////////////////////////////////////////////
int main(int argc, char* argv[])
{
	LoadTestModel(triangles);
	screen = InitializeSDL(SCREEN_WIDTH, SCREEN_HEIGHT);
	t = SDL_GetTicks();	// Set start value for timer.
	while (NoQuitMessageSDL())
	{
		Update();
		Draw();
	}
	SDL_SaveBMP(screen, "screenshot.bmp");
	return 0;
}
//////////////////////////////////////////////
//              UPDATE FUNCTION             //
//////////////////////////////////////////////
void Update()
{
	// Compute frame time:
	int t2 = SDL_GetTicks();
	float dt = float(t2 - t);
	t = t2;
	cout << "Render time: " << dt << " ms." << endl;
}
//////////////////////////////////////////////
//                    DRAW                  //
//////////////////////////////////////////////
void Draw()
{
	if (SDL_MUSTLOCK(screen))
		SDL_LockSurface(screen);

	for (int y = 0; y<SCREEN_HEIGHT; ++y)
	{
		for (int x = 0; x<SCREEN_WIDTH; ++x)
		{
			vec3 screenPos(2 * x / (SCREEN_WIDTH)-1, 1 - 2 * y / (SCREEN_WIDTH), focalLength);
			bool judge = ClosestIntersection(cameraPos, screenPos, triangles, closestIntersection);
			if
			{ 

			}
			vec3 color(1, 0.5, 0.5);
			PutPixelSDL(screen, x, y, color);
		}
	}

	if (SDL_MUSTLOCK(screen))
		SDL_UnlockSurface(screen);

	SDL_UpdateRect(screen, 0, 0, 0, 0);
}
//////////////////////////////////////////////
//   Compare and choose the closest point   //
//////////////////////////////////////////////
bool ClosestIntersection(
	vec3 start,
	vec3 dir,
	const vector<Triangle>& triangles,
	Intersection& closestIntersection
	)
{
	using glm::vec3;
	using glm::mat3;
	vec3 v0 = triangles.v0;
	vec3 v1 = triangles.v1;
	vec3 v2 = triangles.v2;
	vec3 e1 = v1 - v0;
	vec3 e2 = v2 - v0;
	vec3 s = start;
	vec3 b = vec3(s[0] - v0[0], s[1] - v0[1], s[2] - v0[2]);
	vec3 d = unit_vector(s, dir);
	mat3 A (-d, e1, e2);
	vec3 x = glm::inverse(A) * b;
	if ((x[1] > 0) && (x[2] > 0) && ((x[1] + x[2])<1) && (x[0] > 1))
	{
		return true;
	}
	else { return false; }
}
//////////////////////////////////////////////
//         unit vector calculation          //
//////////////////////////////////////////////
vec3 unit_vector(vec3 a, vec3 b)
{
	float leng = sqrt(pow((a[0] - b[0]), 2) + pow((a[1] - b[1]), 2)+pow((a[2] - b[2]), 2));
	float x, y, z;
    x = (b[0] - a[0]) / leng;
	y = (b[1] - a[1]) / leng;
	z = (b[2] - a[2]) / leng;
	vec3 c(x, y, z);
	return c;
}
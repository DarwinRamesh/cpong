#include <raylib.h>
#include <random>

struct screenDimensions{
	float init_sw = 0;
	float init_sh = 0;
	float sw = 0;
	float sh = 0;
};

struct paddleLeft{
	float posx = 20;
	float posy = 20;
	float width = 30;
	float height = 180;
	Color color = WHITE;
	Rectangle paddleLeftBounds;
};

struct paddleRight{
	float posx = 20;
	float posy = 20;
	float width = 30;
	float height = 180;
	Color color = WHITE;
	Rectangle paddleRightBounds;
};

struct progState{
	float dt = 0.0f;
	float gameSpeed = 750.0f;
};

struct puck{
	float posx = 0;
	float posy = 0;
	float width = 15;
	float height = 15;
	Rectangle puckBounds;
	Color WHITE;
	float puckSpeed = 750.0f;
	bool shouldCenter = false;
	bool shouldLeft = true;
	bool shouldRight = false;
	bool hitLeft = false;
	bool hitRight = false;
	bool hitUp = false;
	bool hitDown = false;
	bool shouldDiverge = false;
};

void gameRender(screenDimensions& sd, paddleLeft& pl, paddleRight& pr, puck& pk);
void gameUpdate(screenDimensions& sd, paddleLeft& pl, paddleRight& pr, puck& pk, progState& ps);
void drawPaddleLeft(screenDimensions& sd, paddleLeft& pl);
void drawPaddleRight(screenDimensions& sd, paddleRight& pr);
void captureInput(paddleLeft& pl, paddleRight& pr, progState& ps);
void drawPuck(screenDimensions& sd, puck& pk);
void updatePuck(puck& pk, screenDimensions& sd, progState& ps, paddleLeft& pl, paddleRight& pr);
float genRandVal();
void drawDebug();

int main(){
	screenDimensions sd;
	paddleLeft pl;
	paddleRight pr;
	progState ps;
	puck pk;

	InitWindow(sd.init_sw, sd.init_sh, "Cpong");
	SetTargetFPS(0);

	sd.sw = GetRenderWidth();
	sd.sh = GetRenderHeight();

	pl.posy = (sd.sh / 2.0f) - (pl.height / 2.0f);
	pr.posy = (sd.sh / 2.0f) - (pr.height / 2.0f);
	pk.posy = (sd.sh / 2.0f) - (pk.height / 2.0f);
	pk.posx = (sd.sw / 2.0f) - (pk.width / 2.0f);

	while(!WindowShouldClose()){
		gameUpdate(sd, pl, pr, pk, ps);
		BeginDrawing();
			drawDebug();
			gameRender(sd, pl, pr, pk);
		EndDrawing();
	}
}

void gameRender(screenDimensions& sd, paddleLeft& pl, paddleRight& pr, puck& pk){
	ClearBackground(BLACK);
	drawPaddleLeft(sd, pl);
	drawPaddleRight(sd, pr);
	drawPuck(sd, pk);
}

void gameUpdate(screenDimensions& sd, paddleLeft& pl, paddleRight& pr, puck& pk, progState& ps){
	captureInput(pl, pr, ps);
	updatePuck(pk, sd, ps, pl, pr);
}

void drawPaddleLeft(screenDimensions& sd, paddleLeft& pl){
	pl.paddleLeftBounds = {pl.posx, pl.posy, pl.width, pl.height};
	DrawRectangleRec(pl.paddleLeftBounds, WHITE);
}

void drawPaddleRight(screenDimensions& sd, paddleRight& pr){
	pr.paddleRightBounds = {sd.sw - pr.posx - pr.width, pr.posy, pr.width, pr.height};
	DrawRectangleRec(pr.paddleRightBounds, WHITE);
}

void captureInput(paddleLeft& pl, paddleRight& pr, progState& ps){
	ps.dt = GetFrameTime();

	if(IsKeyDown(KEY_K)){
		pr.posy -= ps.gameSpeed * ps.dt;
	}
	if(IsKeyDown(KEY_J)){
		pr.posy += ps.gameSpeed * ps.dt;
	}if(IsKeyDown(KEY_W)){
		pl.posy -= ps.gameSpeed * ps.dt;
	}
	if(IsKeyDown(KEY_S)){
		pl.posy += ps.gameSpeed * ps.dt;
	}
}

void drawPuck(screenDimensions& sd, puck& pk){
	pk.puckBounds = {pk.posx, pk.posy, pk.width, pk.height};
	DrawRectangleRec(pk.puckBounds, WHITE);
}

void updatePuck(puck& pk, screenDimensions& sd, progState& ps, paddleLeft& pl, paddleRight& pr){
	if(pk.posy < 0){
		pk.posy = 0;
		pk.hitUp = false;
		pk.hitDown = true;
	}
	if(pk.posy + pk.height > sd.sh){
		pk.posy = sd.sh - pk.height;
		pk.hitUp = true;
		pk.hitDown = false;
	}
	if(pk.posx > sd.sw){
		pk.shouldCenter = true;
		pk.shouldLeft = true;
		pk.shouldRight = false;
	}
	if(pk.posx < 0){
		pk.shouldCenter = true;
		pk.shouldLeft = false;
		pk.shouldRight = true;
	}
	if(pk.shouldCenter == true){
		pk.posy = (sd.sh / 2.0f) - (pk.height / 2.0f);
		pk.posx = (sd.sw / 2.0f) - (pk.width / 2.0f);
		pk.hitLeft = false;
		pk.hitRight = false;
		pk.shouldCenter = false;
	}
	if(pk.shouldLeft == true){
		pk.posx -= pk.puckSpeed * ps.dt;
		pk.shouldRight = false;
	}
	if(pk.shouldRight == true){
		pk.posx += pk.puckSpeed * ps.dt;
		pk.shouldLeft = false;
	}
	if(CheckCollisionRecs(pl.paddleLeftBounds, pk.puckBounds) == true){
		pk.shouldLeft = false;
		pk.shouldRight = false;
		pk.hitLeft = false;
		pk.hitRight = true;
		if(genRandVal() > 200.0f){
        		pk.hitUp = true;
        		pk.hitDown = false;
        	}else{
        		pk.hitUp = false;
        		pk.hitDown = true;
        	}
	}	
	if(CheckCollisionRecs(pr.paddleRightBounds, pk.puckBounds) == true){
		pk.shouldLeft = false;
		pk.shouldRight = false;
		pk.hitRight = false;
		pk.hitLeft = true;
		if(genRandVal() > 200.0f){
			pk.hitUp = true;
			pk.hitDown = false;
		}else{
			pk.hitUp = false;
			pk.hitDown = true;
		}
	}
	if(pk.hitLeft == true){
		float high = 400.0f;
		if(pk.hitUp == true){
			pk.posx -= pk.puckSpeed * ps.dt;
			pk.posy -= high * ps.dt;
		}else{
			pk.posx -= pk.puckSpeed * ps.dt;
			pk.posy += high * ps.dt;
		}
	}
	if(pk.hitRight == true){
		float high = 400.0f;
		if(pk.hitUp == true){
			pk.posx += pk.puckSpeed * ps.dt;
			pk.posy -= high * ps.dt;
		}else{
			pk.posx += pk.puckSpeed * ps.dt;
			pk.posy += high * ps.dt;
		}
	}
}

float genRandVal(){
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> distrib(100.0f, 300.0f);
	return distrib(gen); 
}

void drawDebug(){
	DrawText(TextFormat("FPS : %d", GetFPS()), 20, 20, 12, WHITE);
}

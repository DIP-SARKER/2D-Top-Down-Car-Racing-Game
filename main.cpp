#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

const int WIN_W = 560;
const int WIN_H = 800;

//Player
struct Rect { float x, y, w, h; };

Rect player;
float playerSpeed = 6.0f;
bool moveLeft = false, moveRight = false;

//Road
float roadCenterX = WIN_W / 2.0f;
float roadWidth = 360.0f;
float laneMarkW = 6.0f;

//Enemies
struct Enemy { Rect r; float speed; bool counted = false; };
std::vector<Enemy> enemies;

int enemySpawnInterval = 1200;
int lastEnemySpawn = 0;

//Background Trees
float treeScroll1 = 0;
float treeScroll2 = 800;
float treeSpeed   = 1.5f;

//Timing & Game State
int lastUpdateTime = 0;
int score = 0;
bool gameOver = false;

//Others
int getTimeMs(){ return glutGet(GLUT_ELAPSED_TIME); }

float randf(float a, float b){ return a + (rand()/(float)RAND_MAX)*(b-a); }

void drawRect(const Rect &r){
    glBegin(GL_QUADS);
      glVertex2f(r.x,       r.y);
      glVertex2f(r.x+r.w,   r.y);
      glVertex2f(r.x+r.w,   r.y+r.h);
      glVertex2f(r.x,       r.y+r.h);
    glEnd();
}

void drawText(float x, float y, const std::string &s){
    glRasterPos2f(x, y);
    for(char c: s) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
}

bool rectOverlap(const Rect &a, const Rect &b){
    return !(a.x + a.w < b.x || b.x + b.w < a.x ||
             a.y + a.h < b.y || b.y + b.h < a.y);
}

void resetGame(){
    enemies.clear();
    score = 0;
    gameOver = false;

    player.w = 36;
    player.h = 60;
    player.x = roadCenterX - player.w/2.0f;
    player.y = 60;

    lastEnemySpawn = getTimeMs();
    lastUpdateTime = getTimeMs();
}

//Spawn Enemies
void spawnEnemy(){
    Enemy e;
    e.r.w = 36;
    e.r.h = 60;

    float roadL = roadCenterX - roadWidth/2.0f;
    float laneWidth = roadWidth / 4.0f;

    int lane = rand() % 4;
    float laneCenter = roadL + laneWidth * lane + laneWidth/2.0f;

    e.r.x = laneCenter - e.r.w/2.0f;
    e.r.y = WIN_H + 20;

    e.speed = randf(2.0f, 3.5f);
    enemies.push_back(e);
}

//Draw Trees
void drawTrees(float yOffset){
    glColor3f(0.0f, 0.75f, 0.0f);

    const float leftX  = 25;
    const float rightX = WIN_W - 25;
    const float spacingY = 70;
    const float treeH = 40;
    const float treeW = 25;

    for(int i=0; i<15; i++){
        float y = yOffset + i * spacingY;

        // Left
        glBegin(GL_TRIANGLES);
            glVertex2f(leftX,            y + treeH);
            glVertex2f(leftX - treeW/2,  y);
            glVertex2f(leftX + treeW/2,  y);
        glEnd();

        // Right
        glBegin(GL_TRIANGLES);
            glVertex2f(rightX,            y + treeH);
            glVertex2f(rightX - treeW/2,  y);
            glVertex2f(rightX + treeW/2,  y);
        glEnd();
    }
}

//DDA
void drawDDALine(float x1, float y1, float x2, float y2, int dashLen = 10, int gapLen = 2){
    float dx = x2 - x1;
    float dy = y2 - y1;

    float steps = std::max(fabs(dx), fabs(dy));
    if(steps < 1.0f){
        glBegin(GL_POINTS);
          glVertex2f(x1, y1);
        glEnd();
        return;
    }

    float xInc = dx / steps;
    float yInc = dy / steps;

    float x = x1;
    float y = y1;

    int counter = 0;
    bool drawing = true;
    int patternLen = dashLen + gapLen;

    glBegin(GL_POINTS);
    for(int i=0; i<= (int)steps; ++i){
        int m = counter % patternLen;
        if(m < dashLen){
            glVertex2f(x, y);
        }
        x += xInc;
        y += yInc;
        counter++;
    }
    glEnd();
}

//Update
void updateScene(){
    int now = getTimeMs();
    int dt = now - lastUpdateTime;
    if(dt <= 0) return;
    lastUpdateTime = now;

    if(!gameOver){

        if(moveLeft)  player.x -= playerSpeed;
        if(moveRight) player.x += playerSpeed;

        float leftBound  = roadCenterX - roadWidth/2.0f + 6;
        float rightBound = roadCenterX + roadWidth/2.0f - player.w - 6;

        if(player.x < leftBound)  player.x = leftBound;
        if(player.x > rightBound) player.x = rightBound;

        if(now - lastEnemySpawn > enemySpawnInterval){
            spawnEnemy();
            enemySpawnInterval = 700 + rand() % 1200;
            lastEnemySpawn = now;
        }

        treeScroll1 -= treeSpeed;
        treeScroll2 -= treeSpeed;

        if(treeScroll1 <= -WIN_H) treeScroll1 = WIN_H;
        if(treeScroll2 <= -WIN_H) treeScroll2 = WIN_H;

        for(auto &e : enemies){
            e.r.y -= e.speed * (dt / 16.0f) * 6.0f;

            if(!e.counted && e.r.y + e.r.h < player.y){
                score++;
                e.counted = true;
            }
        }

        enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                         [](const Enemy &e){ return e.r.y + e.r.h < 0; }),
                         enemies.end());

        for(const auto &e : enemies){
            if(rectOverlap(player, e.r)){
                gameOver = true;
            }
        }
    }
}

//Display
void display(){
    glClearColor(0.2f, 0.8f, 0.95f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Grass
    glColor3f(0.15f, 0.6f, 0.15f);
    glBegin(GL_QUADS);
      glVertex2f(0,     0);
      glVertex2f(WIN_W, 0);
      glVertex2f(WIN_W, WIN_H);
      glVertex2f(0,     WIN_H);
    glEnd();

    // Trees
    drawTrees(treeScroll1);
    drawTrees(treeScroll2);

    // Road
    float roadL = roadCenterX - roadWidth/2.0f;
    float roadR = roadCenterX + roadWidth/2.0f;

    glColor3f(0.18f, 0.18f, 0.18f);
    glBegin(GL_QUADS);
      glVertex2f(roadL, 0);
      glVertex2f(roadR, 0);
      glVertex2f(roadR, WIN_H);
      glVertex2f(roadL, WIN_H);
    glEnd();


    glPointSize(laneMarkW);
    for(int i=1; i<4; i++){
        float x = roadL + (roadWidth/4.0f) * i;
        glColor3f(1.0f, 1.0f, 1.0f);
        drawDDALine(x, 0.0f, x, (float)WIN_H, 18,10);
    }
    glPointSize(1.0f);

    // Enemies
    for(const auto &e : enemies){
        glColor3f(0.75f, 0.1f, 0.1f);
        drawRect(e.r);

        glColor3f(0.85f, 0.9f, 1.0f);
        Rect w = { e.r.x+6, e.r.y+e.r.h-20, e.r.w-12, 14 };
        drawRect(w);
    }

    // Player
    glColor3f(0.05f, 0.6f, 0.05f);
    drawRect(player);

    glColor3f(0.85f, 0.95f, 1.0f);
    Rect pw = { player.x+6, player.y+player.h-20, player.w-12, 14 };
    drawRect(pw);

    // Score
    glColor3f(1,1,1);
    drawText(10, WIN_H-24, "Score: " + std::to_string(score));

    if(gameOver){
        std::string msg = "GAME OVER - Press R to restart";
        float tx = WIN_W/2 - msg.size()*9/2;
        glColor3f(1,0.8f,0.2f);
        drawText(tx, WIN_H/2, msg);
    }

    glutSwapBuffers();
}

//input
void timerFunc(int){
    updateScene();
    glutPostRedisplay();
    glutTimerFunc(16, timerFunc, 0);
}

void specialKey(int key, int, int){
    if(key == GLUT_KEY_LEFT)  moveLeft = true;
    if(key == GLUT_KEY_RIGHT) moveRight = true;
}

void specialKeyUp(int key, int, int){
    if(key == GLUT_KEY_LEFT)  moveLeft = false;
    if(key == GLUT_KEY_RIGHT) moveRight = false;
}

void keyboard(unsigned char key, int, int){
    if(key == 'r' || key == 'R') resetGame();
}

void reshape(int w, int h){
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIN_W, 0, WIN_H);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv){
    srand((unsigned)time(nullptr));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WIN_W, WIN_H);
    glutCreateWindow("Top-Down Car Racer (GLUT)");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKey);
    glutSpecialUpFunc(specialKeyUp);
    glutKeyboardFunc(keyboard);

    resetGame();
    lastUpdateTime = getTimeMs();
    glutTimerFunc(16, timerFunc, 0);

    glutMainLoop();
    return 0;
}

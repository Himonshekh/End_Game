#include<bits/stdc++.h>
#include<windows.h>
#include<stdio.h>
#include<iostream>
#include<GL/glut.h>
#include<string>
#include<sstream>
#include <GL/gl.h>

using namespace std;
float colors[10][3] ={{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0},
                      {0.0, 0.0, 1.0}, {1.0, 1.0, 0.0},
                      {0.0, 0.0, 0.0}, {0.0, 0.39, 0.0},
                      {0.29, 0.0, 0.5},{1.0, 0.0, 1.0}};

float sWidth=600.0,sHeight=600.0,scaleX=500.0,scaleY=500.0,centerX,centerY;
float shipX1,shipX2,shipX3,shipX4,shipX5,shipY1,shipY2,shipY3,shipY4,shipY5;
float enemySpeed=0.1;
bool isMoveBullet=false,isMoveEnemy=true,isGameOver=false;
int roundScore=0;
struct Ship{
    float shipX1,shipX2,shipX3,shipX4,shipX5,shipY1,shipY2,shipY3,shipY4,shipY5;
};
Ship ship;

struct Bullet{
    float x;
    float y;
    bool isAlive= false;
};
Bullet bullets[10];

struct Enemy{
    float X1,X2,X3,X4,Y1,Y2,Y3,Y4;
    bool isAlive=true;
};
Enemy enemy [100];

void printScore(int score){

    stringstream strstrm;
    strstrm << score;
    string str = "Score: "+strstrm.str();
    int len = str.length();
    for(int i = 0; i < len; i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, str[i]);
    }
}
void winningText(){
    glColor3f (1.0, 0.0, 0.0);
    glRasterPos2f(50.0f,50.0f);
    string str="LEVEL 1 COMPLETE!!";
    int len=str.length();
    for(int i=0;i<len;i++){
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13,str[i]);
    }
    cout<<str<<endl;
}

void myDisplay(void){
    glClear (GL_COLOR_BUFFER_BIT);
    //glMatrixMode(GL_MODELVIEW);
    glBegin(GL_POLYGON);
        glColor3f (0.0, 0.0, 1.0);
        glVertex3f(ship.shipX1,ship.shipY1, 0.0f);
        glColor3f (.0, 1.0, 0.0);
        glVertex3f(ship.shipX2,ship.shipY2, 0.0f);
        glColor3f (0.0, 1.0, 1.0);
        glVertex3f(ship.shipX3,ship.shipY3,0.0f);
        glColor3f (1.0, 0.0, 0.0);
        glVertex3f(ship.shipX4,ship.shipY4,0.0f);
        glColor3f (1.0, 0.0, 1.0);
        glVertex3f(ship.shipX5,ship.shipY5, 0.0f);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
        for(int j=0;j<10;j++){
            if(!bullets[j].isAlive){
                continue;
            }
            glVertex2f(bullets[j].x,bullets[j].y);
            for(float i = 0.0f; i <= 360; i++)
                glVertex2f(7*cos(M_PI * i / 180.0) + bullets[j].x, 7*sin(M_PI * i / 180.0) + bullets[j].y);
        }
    glEnd();

    glBegin(GL_QUADS);
        for(int i=0;i<100;i++){
            glColor3f (colors[i%8][0],colors[i%8][1],colors[i%8][2]);
            glVertex3f(enemy[i].X1,enemy[i].Y1,0.0);
            glVertex3f(enemy[i].X2,enemy[i].Y2,0.0);
            glVertex3f(enemy[i].X3,enemy[i].Y3,0.0);
            glVertex3f(enemy[i].X4,enemy[i].Y4,0.0);
        }
    glEnd();
    glutSwapBuffers();
}


void loadBullet(){
    int id=-1;
    for(int i=0;i<10;i++){
        if(!bullets[i].isAlive){
            if(id<0)id=i;
        }else{
            if(bullets[i].y>500){
                bullets[i].isAlive=false;

            }
        }
    }
    bullets[id].x=ship.shipX1;bullets[id].y=ship.shipY1;
    bullets[id].isAlive=true;
    //cout<<id<<" active "<<bullets[id].x<<" "<<bullets[id].y<<endl;
}

void moveBullet(){
    for(int i=0;i<10;i++){
        if(bullets[i].isAlive){
            bullets[i].y+=7;
        }
        if(bullets[i].y>500)bullets[i].isAlive=false;
    }
    glutPostRedisplay();
}

void moveEnemy(){
    for(int i=0;i<100;i++){
        if(!enemy[i].isAlive)continue;
        enemy[i].Y1-=enemySpeed;enemy[i].Y2-=enemySpeed;enemy[i].Y3-=enemySpeed;enemy[i].Y4-=enemySpeed;
    }
    glutPostRedisplay();
}

void gameOver(){
    for(int i=0;i<100;i++){
        if(enemy[i].Y3<=-500){
            isGameOver=true;return;
        }
    }
    if(roundScore==100)isGameOver=true;
}

void updateScore(int score){
    if(score){
        roundScore+=score;
        cout<<roundScore<<endl;
    }
    printScore(roundScore);
    if(roundScore==100){
        //isGameOver=true;
        winningText();
    }
}

void checkCollision(){
    int l_score=0;
    for(int i=0;i<10;i++){
        if(bullets[i].isAlive){
            for(int j=0;j<100;j++){
                if(!enemy[j].isAlive)continue;
                if((bullets[i].x >= enemy[j].X4 && bullets[i].x<=enemy[j].X3 )&&
                   (bullets[i].y >= enemy[j].Y4 && bullets[i].y <= enemy[j].Y1)){
                        bullets[i].y=-scaleY-100;bullets[i].isAlive=false;
                        enemy[j].Y1=600; enemy[j].Y2=600; enemy[j].Y3=600; enemy[j].Y4=600;enemy[j].isAlive=false;
                        l_score++;
                        break;
                }
            }
        }
    }
    updateScore(l_score);
}


void startMovement(){
    checkCollision();
    gameOver();
    if(isMoveBullet)moveBullet();
    if(isMoveEnemy)moveEnemy();
    if(isGameOver){
        glutIdleFunc(NULL);return;
    }
}


void movement(){
    glutIdleFunc(NULL);
    glutIdleFunc(startMovement);
    if(isGameOver)return;
}

void displayInfo(){
    for(int i=0;i<100;i++){
        cout<<enemy[i].X4<<" "<<enemy[i].Y4<<endl;
    }
}

void charKeyboard(unsigned char key, int x, int y){
    if(isGameOver)return;
    switch(key){
        case 32:
            loadBullet();
            isMoveBullet=true;
            movement();
            break;
        case 's':
            cout<<"stop"<<endl;
            glutIdleFunc(NULL);
            break;
        case 'o':
            displayInfo();
            break;
        default:
            break;
    }
}


void keyboard(int key, int x, int y){
    if(isGameOver)return;
    switch(key){
        case GLUT_KEY_LEFT:
            ship.shipX1 -= 15;ship.shipX2 -= 15;ship.shipX3 -= 15;ship.shipX4 -= 15;ship.shipX5 -= 15;
            if(ship.shipX1 < -500){
                ship.shipX1 += 15;ship.shipX2 += 15;ship.shipX3 += 15;ship.shipX4 += 15;ship.shipX5 += 15;
            }
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT:
            ship.shipX1 += 15;ship.shipX2 += 15;ship.shipX3 += 15;ship.shipX4 += 15;ship.shipX5 += 15;
            if(ship.shipX1 > 500){
                ship.shipX1 -= 15;ship.shipX2 -= 15;ship.shipX3 -= 15;ship.shipX4 -= 15;ship.shipX5 -= 15;
            }
            glutPostRedisplay();
            break;
        default:
            break;
    }
}


void initShipPosition(){
    ship.shipX1=centerX; ship.shipX2=centerX+50; ship.shipX3=centerX+25; ship.shipX4=centerX-25; ship.shipX5=centerX-50;
    ship.shipY1=centerY+100;ship.shipY2=centerY;ship.shipY3=centerY+25;ship.shipY4=centerY+25;ship.shipY5=centerY;
}


void initPosition(){
    for(int i=0;i<20;i++){
        bullets[i].x=0.0;bullets[i].y=-scaleY-100;bullets[i].isAlive=false;
    }
    srand(time(0));
    for(int i=0;i<100;i++){
            int randX= (rand() % 900)-450;
            int randY= (rand() % 3000)+800;
            enemy[i].X1=randX;enemy[i].Y1=randY;
            enemy[i].X2=randX+50;enemy[i].Y2=randY;
            enemy[i].X3=randX+50;enemy[i].Y3=randY-50;
            enemy[i].X4=randX;enemy[i].Y4=randY-50;
            enemy[i].isAlive=true;
    }
}


void init(void){
    //glShadeModel (GL_SMOOTH);
    glClearColor(0.1f, 0.0f, 0.1f, 0.0);
    glColor3f(0.0f, 0.0f, 0.0f);
    glOrtho(-scaleX, scaleX,-scaleY , scaleY, -1.0, 1.0);
    centerX=0;centerY=-scaleY+20;
    initShipPosition();
    initPosition();

    //glMatrixMode(GL_PROJECTION);
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();

}

int main(int argc, char** argv){
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(sWidth,sHeight);
    glutInitWindowPosition(300,40);
    glutCreateWindow("End Game");
    init();
    glutDisplayFunc(myDisplay);
    glutSpecialFunc(keyboard);
    glutKeyboardFunc(charKeyboard);
    movement();
    //glutMouseFunc(mouse);
    glutMainLoop();
}

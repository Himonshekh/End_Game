/*
        ________INTERVAL________
          09 November 2021 14:45:10
*/
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
bool isMoveBullet=false,isMoveEnemy=true,isGameOver=false,isWin=false;
int roundScore=0,currentLevelIndex;
struct Ship{
    float shipX1,shipX2,shipX3,shipX4,shipX5,shipY1,shipY2,shipY3,shipY4,shipY5,speed;
};
Ship ship;

struct Level{
    int noOfEnemies,remainingEnemies;
    float enemySpeed;
    int levelNo;
};

Level levels[10],currentLevel;

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

void winningText(int flg=0){
    if(!flg)return;
    glColor3f (1.0, 0.0, 0.0);
    glRasterPos2f(-250,0);
    string str="Congratulations!!! LEVEL 1 COMPLETE!!";
    int len=str.length();
    cout<<str<<endl;
    for(int i=0;i<len;i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,str[i]);
    }
}


void initLevels(){
    levels[0].noOfEnemies = 2;
    levels[0].enemySpeed = 0.5;
    levels[0].levelNo = 1;
    levels[1].noOfEnemies = 3;
    levels[1].enemySpeed = 1;
    levels[1].levelNo = 2;
    levels[2].noOfEnemies = 5;
    levels[2].enemySpeed = 1.5;
    levels[2].levelNo = 3;

    levels[3].noOfEnemies = 6;
    levels[3].enemySpeed = 2.0;

    levels[4].noOfEnemies = 7;
    levels[4].enemySpeed = 2.5;

    levels[5].noOfEnemies = 8;
    levels[5].enemySpeed = 3;

    levels[6].noOfEnemies = 10;
    levels[6].enemySpeed = 3.75;

    currentLevelIndex = 0;

    currentLevel = levels[currentLevelIndex];
    currentLevel.remainingEnemies = currentLevel.noOfEnemies;
}


void levelUp(){
    winningText(1);
    currentLevelIndex++;
    currentLevel = levels[currentLevelIndex];
    currentLevel.remainingEnemies = currentLevel.noOfEnemies;
    currentLevel.levelNo = currentLevelIndex+1;
}

void printScore(int score){
    glColor3f(0.0, 1.0, 0.0);
    glRasterPos2f(scaleX-160,scaleY-70);
    stringstream strstrm,strstrm2,strm3;
    strstrm << score;
    strstrm2 << currentLevel.levelNo;
    strm3 << currentLevel.remainingEnemies;
    string str = "Score: "+strstrm.str();
    string levelString = "Level : "+strstrm2.str();
    string enemyInfo =  "\nEnemy Left :"+strm3.str();

    int len=str.length();
    for(int i=0;i<len;i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,str[i]);
    }
    glRasterPos2f(-scaleX+10,scaleY-70);
    len=levelString.length();
    for(int i=0;i<len;i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,levelString[i]);
    }

    glRasterPos2f(-scaleX+10.
                  ,scaleY-100);
    len=enemyInfo.length();
    for(int i=0;i<len;i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,enemyInfo[i]);
    }

    //glutBitmapString(GLUT_BITMAP_HELVETICA_18,"Fuck you glut");
    if(score==100){
        glutIdleFunc(NULL);
        winningText(1);
        isWin=true;
    }
}


void deadText(int flg=0){
    if(!flg)return;
    glColor3f (1.0, 0.0, 0.0);
    glRasterPos2f(-250,0);
    string str="Game Over!!! Try Again";
    int len=str.length();
    cout<<str<<endl;
    for(int i=0;i<len;i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,str[i]);
    }
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
    printScore(roundScore);
    winningText(isWin==true);
    deadText(isGameOver==true);
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
            bullets[i].y+=10;
        }
        if(bullets[i].y>500)bullets[i].isAlive=false;
    }
    glutPostRedisplay();
}

void moveEnemy(){
    for(int i=0;i<100;i++){
        if(!enemy[i].isAlive)continue;
        enemy[i].Y1-=currentLevel.enemySpeed;
        enemy[i].Y2-=currentLevel.enemySpeed;
        enemy[i].Y3-=currentLevel.enemySpeed;
        enemy[i].Y4-=currentLevel.enemySpeed;
    }
    glutPostRedisplay();
}

void gameOver(){
    for(int i=0;i<100;i++){
        if(enemy[i].Y3<=-500){
            isGameOver=true;
            break;
        }
    }
    if(isGameOver){
        glutIdleFunc(NULL);
        deadText(1);
    }
}

void updateScore(int score){
    if(score){
        roundScore+=score;
        cout<<roundScore<<endl;
    }
    printScore(roundScore);
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
                        currentLevel.remainingEnemies--;
                        if(currentLevel.remainingEnemies==0){
                            levelUp();
                        }
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
}


void movement(){
    glutIdleFunc(NULL);
    if(isGameOver||isWin)return;
    glutIdleFunc(startMovement);
}

void displayInfo(){
    for(int i=0;i<100;i++){
        cout<<enemy[i].X4<<" "<<enemy[i].Y4<<endl;
    }
}

void charKeyboard(unsigned char key, int x, int y){
    if(isGameOver||isWin)return;
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
    if(isGameOver||isWin)return;
    switch(key){
        case GLUT_KEY_LEFT:
            ship.shipX1 -= ship.speed;ship.shipX2 -= ship.speed;ship.shipX3 -= ship.speed;ship.shipX4 -= ship.speed;ship.shipX5 -= ship.speed;
            if(ship.shipX1 < -500){
                ship.shipX1 += ship.speed;ship.shipX2 += ship.speed;ship.shipX3 += ship.speed;ship.shipX4 += ship.speed;ship.shipX5 += ship.speed;
            }
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT:
            ship.shipX1 += ship.speed;ship.shipX2 += ship.speed;ship.shipX3 += ship.speed;ship.shipX4 += ship.speed;ship.shipX5 += ship.speed;
            if(ship.shipX1 > 500){
                ship.shipX1 -= ship.speed;ship.shipX2 -= ship.speed;ship.shipX3 -= ship.speed;ship.shipX4 -= ship.speed;ship.shipX5 -= ship.speed;
            }
            glutPostRedisplay();
            break;
        default:
            break;
    }
}


void initShipPosition(){
    ship.speed=15;
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
    initLevels();
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

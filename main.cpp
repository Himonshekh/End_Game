#include<bits/stdc++.h>
#include<windows.h>
#include<stdio.h>
#include<iostream>
#include<GL/glut.h>
#include<string>
#include<sstream>
#include <GL/gl.h>

using namespace std;

float colors[10][3] ={{1.0, 0.2, 0.0}, {0.4, 0.5, 0.7},
                      {0.0, 0.0, 1.0}, {1.0, 1.0, 0.4},
                      {0.4, 0.5, 0.8}, {0.7, 0.39, 0.0},
                      {0.29, 0.8, 0.5},{1.0, 0.0, 1.0}};

float sWidth=600.0,sHeight=600.0,scaleX=500.0,scaleY=500.0,centerX,centerY;
float shipX1,shipX2,shipX3,shipX4,shipX5,shipY1,shipY2,shipY3,shipY4,shipY5;
float colorR=.9,colorG=0.0,colorB=0.9;
bool isMoveBullet=false,isMoveEnemy=true,isGameOver=false,level_up_flag=false,mission_passed_flag=false;
int roundScore=0,currentLevelIndex;

struct Ship{
    float shipX1,shipX2,shipX3,shipX4,shipX5,shipY1,shipY2,shipY3,shipY4,shipY5,speed;
};
Ship ship;

struct Bullet{
    float x, y;
    bool isAlive= false;
};
Bullet bullets[11];

struct Enemy{
    float X1,X2,X3,X4,Y1,Y2,Y3,Y4;
    bool isAlive=true;
};
Enemy enemy [101];

struct PowerJelly{
    float x1,x2,x3,x4,x5,x6,x7,x8,y1,y2,y3,y4,y5,y6,y7,y8;
    bool isAlive=false;
};
PowerJelly power_ball[11];

struct Level{
    int noOfEnemies,remainingEnemies;
    float enemySpeed,power_ball_speed,jellyFishPower;
    int levelNo;
};
Level levels[11],currentLevel;

void displayInfo(){
    for(int i=0;i<currentLevel.noOfEnemies;i++){
            cout<<bullets[i].x<<" "<<bullets[i].y<<endl;
        //cout<<enemy[i].X4<<" "<<enemy[i].Y4<<" "<<enemy[i].isAlive<<endl;
    }
}


void resetGame(){
    currentLevel.jellyFishPower=0;
    srand(time(0));
    int modY=50;
    for(int i=0;i<10;i++){
        bullets[i].x=0.0;bullets[i].y=-scaleY-100;bullets[i].isAlive=false;
        power_ball[i].isAlive=false;
    }

    for(int i=0;i<currentLevel.levelNo;i++){
        int randX= (rand() % 900)-450;
        int randY= (rand() % (modY*currentLevel.noOfEnemies)) +700;

        power_ball[i].x1=randX;power_ball[i].y1=randY+12;
        power_ball[i].x2=randX+20;power_ball[i].y2=randY+20;
        power_ball[i].x3=randX+12;power_ball[i].y3=randY;
        power_ball[i].x4=randX+20;power_ball[i].y4=randY-20;
        power_ball[i].x5=randX;power_ball[i].y5=randY-12;
        power_ball[i].x6=randX-20;power_ball[i].y6=randY-20;
        power_ball[i].x7=randX-12;power_ball[i].y7=randY;
        power_ball[i].x8=randX-20;power_ball[i].y8=randY+20;
        if(currentLevel.levelNo>1)power_ball[i].isAlive=true;
    }

    cout<<"enemy :"<<currentLevel.noOfEnemies<<" | enemy speed:"<<currentLevel.enemySpeed<<" |ship speed :"<<ship.speed<<endl;
    for(int i=0;i<currentLevel.noOfEnemies;i++){
            int randX= (rand() % 900)-450;
            int randY= (rand() % (modY*currentLevel.noOfEnemies))+600;

            enemy[i].X1=randX;enemy[i].Y1=randY;
            enemy[i].X2=randX+40;enemy[i].Y2=randY;
            enemy[i].X3=randX+40;enemy[i].Y3=randY-40;
            enemy[i].X4=randX;enemy[i].Y4=randY-40;
            enemy[i].isAlive=true;
    }
    //isWin=false;
}

void levelUpText(){
    // tanjila
    if(!level_up_flag)return;
    glColor3f(colorR, colorG, colorB);//0.1f, 0.0f, 0.1f,
    glRasterPos2f(-130,0);
    string str="LEVEL COMPLETE!!!";
    int len=str.length();
    for(int i=0;i<len;i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,str[i]);
    }
    colorB-=.0008;colorR-=.0008;
    if(colorB<.2){
        level_up_flag=false;
        colorB=colorR=.9;
    }
}

void levelUp(){
    // sazzad
    level_up_flag=true;
    currentLevelIndex++;
    if(currentLevelIndex>=10){
        mission_passed_flag=true;
        currentLevelIndex=0;
    }
    ship.speed+=3;
    if(ship.speed>30)ship.speed=30;
    currentLevel = levels[currentLevelIndex];
    currentLevel.remainingEnemies = currentLevel.noOfEnemies;
    currentLevel.levelNo = currentLevelIndex+1;
    resetGame();
    glutPostRedisplay();
}

void printScore(int score){
    //tanjila
    glColor3f(0.0, 1.0, 0.0);
    glRasterPos2f(scaleX-160,scaleY-50);
    stringstream strstrm,strstrm2,strm3;
    strstrm << score;
    strstrm2 << currentLevel.levelNo;
    strm3 << currentLevel.remainingEnemies;
    string str = "Score: "+strstrm.str();
    string levelString = "Level : "+strstrm2.str();
    string enemyInfo =  "Enemy Left :"+strm3.str();

    int len=str.length();
    for(int i=0;i<len;i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,str[i]);
    }

    glRasterPos2f(-scaleX+10,scaleY-50);
    len=levelString.length();
    for(int i=0;i<len;i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,levelString[i]);
    }

    glRasterPos2f(-scaleX+10.
                  ,scaleY-80);
    len=enemyInfo.length();
    for(int i=0;i<len;i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,enemyInfo[i]);
    }
}

string getTopScore(){
    //suronjit
    string file_score="";
    ifstream file_read ("C:/Users/Himon/Desktop/run/input.txt");
    if (file_read.is_open()){
        getline (file_read,file_score);
        file_read.close();
    }
    reverse(file_score.begin(),file_score.end());
    return file_score;
}

string getRoundScore(){
    // suronjit
    string str="";
    int t_roundScore=roundScore;
    if(t_roundScore==0){
        str="0";
    }
    while(t_roundScore){
        char dig=(t_roundScore%10)+'0';
        str=str+dig;
        t_roundScore/=10;
    } //123=321 =123

    reverse(str.begin(),str.end());
    return str;
}

void printCompareScore(){
    //suronjit
    glColor3f(0.0, 1.0, 0.0);
    glRasterPos2f(-120,250);
    string str="Your Score: "+getRoundScore();
    int len=str.length();
    for(int i=0;i<len;i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,str[i]);
    }

    glColor3f(0.0, 1.0, 0.0);
    glRasterPos2f(-150,300);
    str="Highest Score: "+getTopScore();
    len=str.length();
    for(int i=0;i<len;i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,str[i]);
    }
}

void compareTopScore(){
    //suronjit
    string file_score="";
    int top_score=0;
    ifstream file_read ("C:/Users/Himon/Desktop/run/input.txt");
    if (file_read.is_open()){
        getline (file_read,file_score);
        file_read.close();
    }
    int len=file_score.length(); // 123 // 321 // 12*10+3=123
    for(int i=len-1; i>=0; i--){
        top_score=top_score*10 + (file_score[i]-'0');
    }
    if(top_score<roundScore){
        ofstream file_write;
        file_write.open ("C:/Users/Himon/Desktop/run/input.txt");
        file_score="";
        top_score=roundScore;
        while(top_score){
            char dig=(top_score%10)+'0';
            file_score =file_score+dig;
            top_score/=10;
        }
        file_write << file_score;
        file_write.close();
    }
    printCompareScore();
}

void deadText(int flg=0){
    // tanjila
    if(!flg)return;
    glColor3f (1.0, 0.0, 0.0);
    glRasterPos2f(-120,100);
    string str="Game Over!!!";
    int len=str.length();
    cout<<str<<endl;
    for(int i=0;i<len;i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,str[i]);
    }

    glRasterPos2f(-180,50);
    str="Restart ? PRESS R";
    len=str.length();
    for(int i=0;i<len;i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,str[i]);
    }
    compareTopScore();
}

void missionPassed(){
    //tanjila
    if(!mission_passed_flag)return;
    glColor3f (1.0, 0.0, 0.0);
    glRasterPos2f(-130,100);
    string str="MISSION PASSED!!!";
    int len=str.length();
    cout<<str<<endl;
    for(int i=0;i<len;i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,str[i]);
    }
}

void myDisplay(void){
    //printf("myDisplay\n");
    glClear (GL_COLOR_BUFFER_BIT);

    /*
        ship co-ordinate
                      x1


                   x4   x3
         x5                     x2
    */

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

    /*
        power ball co-ordinate
        x8              x2
                x1

            x7       x3

                x5

        x6              x4
    */
    for(int i=0;i<10;i++){
        if(!power_ball[i].isAlive)continue;
        glBegin(GL_POLYGON);
            glColor3f (0.0, 0.0, 1.0);
            glVertex3f(power_ball[i].x1,power_ball[i].y1, 0.0f);
            glColor3f (.0, 1.0, 0.0);
            glVertex3f(power_ball[i].x2,power_ball[i].y2, 0.0f);
            glColor3f (0.0, 1.0, 1.0);
            glVertex3f(power_ball[i].x3,power_ball[i].y3,0.0f);
            glColor3f (1.0, 0.0, 0.0);
            glVertex3f(power_ball[i].x4,power_ball[i].y4,0.0f);
            glColor3f (1.0, 0.0, 1.0);
            glVertex3f(power_ball[i].x5,power_ball[i].y5, 0.0f);
            glColor3f (0.10, 0.0, 0.10);
            glVertex3f(power_ball[i].x6,power_ball[i].y6, 0.0f);
            glColor3f (0.10, 0.10, 1.0);
            glVertex3f(power_ball[i].x7,power_ball[i].y7, 0.0f);
            glColor3f (0.20, 0.30, 0.5);
            glVertex3f(power_ball[i].x8,power_ball[i].y8, 0.0f);
        glEnd();
    }

    glColor3f (1.0, 1.0, 1.0);
    for(int j=0;j<10;j++){
        if(!bullets[j].isAlive)continue;
        glBegin(GL_TRIANGLE_FAN);
            glVertex2f(bullets[j].x,bullets[j].y);
            for(float i = 0.0f; i <= 360; i++)
                glVertex2f(7*cos(M_PI * i / 180.0) + bullets[j].x, 7*sin(M_PI * i / 180.0) + bullets[j].y);

        glEnd();
    }

    /*
        enemy co-ordinate

        x1y1    x2y2

        x4y4    x3y3

    */
    glBegin(GL_QUADS);
        for(int i=0;i<currentLevel.noOfEnemies;i++){
            if(!enemy[i].isAlive)continue;
            glColor3f (colors[i%8][0],colors[i%8][1],colors[i%8][2]);
            glVertex3f(enemy[i].X1,enemy[i].Y1,0.0);
            glVertex3f(enemy[i].X2,enemy[i].Y2,0.0);
            glVertex3f(enemy[i].X3,enemy[i].Y3,0.0);
            glVertex3f(enemy[i].X4,enemy[i].Y4,0.0);
        }
    glEnd();
    printScore(roundScore);
    levelUpText();
    missionPassed();
    deadText(isGameOver==true);
    glutSwapBuffers();
}


void moveBullet(){
    for(int i=0;i<10;i++){
        if(bullets[i].isAlive){
            bullets[i].y+=10;
        }
        if(bullets[i].y>500)bullets[i].isAlive=false;
    }
}

void loadBulletWithPower(){
    //sazzad
    int cnt=3;
    for(int i=0;i<10&&cnt;i++){
        if(!bullets[i].isAlive){
            if(cnt==3){
                bullets[i].x=ship.shipX1;bullets[i].y=ship.shipY1;
                bullets[i].isAlive=true;
            }else if(cnt==2){
                bullets[i].x=ship.shipX1-40;bullets[i].y=ship.shipY1;
                bullets[i].isAlive=true;
            }else{
                bullets[i].x=ship.shipX1+40;bullets[i].y=ship.shipY1;
                bullets[i].isAlive=true;
            }
            cnt--;
        }
    }
}

void loadBullet(){
    if(currentLevel.jellyFishPower){
        loadBulletWithPower();
        currentLevel.jellyFishPower--;
        return;
    }
    int id=-1;
    for(int i=0;i<10;i++){
        if(!bullets[i].isAlive){
            id=i;break;
        }
    }
    bullets[id].x=ship.shipX1;bullets[id].y=ship.shipY1;
    bullets[id].isAlive=true;
    //cout<<id<<" active "<<bullets[id].x<<" "<<bullets[id].y<<endl;
}


void moveEnemy(){
    if(mission_passed_flag)return;
    //cout<<" move enemy "<<currentLevel.noOfEnemies<<endl;
    for(int i=0;i<currentLevel.noOfEnemies;i++){
        if(!enemy[i].isAlive)continue;
        enemy[i].Y1-=currentLevel.enemySpeed;
        enemy[i].Y2-=currentLevel.enemySpeed;
        enemy[i].Y3-=currentLevel.enemySpeed;
        enemy[i].Y4-=currentLevel.enemySpeed;
    }
    //displayInfo();
    glutPostRedisplay();
}

void movePower(){
    //jelly fish power movement
    if(mission_passed_flag)return;
    //cout<<"power ball"<<endl;
    for(int i=0;i<10;i++){
        if(!power_ball[i].isAlive)continue;
        power_ball[i].y1-=currentLevel.power_ball_speed;
        power_ball[i].y2-=currentLevel.power_ball_speed;
        power_ball[i].y3-=currentLevel.power_ball_speed;
        power_ball[i].y4-=currentLevel.power_ball_speed;
        power_ball[i].y5-=currentLevel.power_ball_speed;
        power_ball[i].y6-=currentLevel.power_ball_speed;
        power_ball[i].y7-=currentLevel.power_ball_speed;
        power_ball[i].y8-=currentLevel.power_ball_speed;
    }
    glutPostRedisplay();
}

void gameOver(){
    for(int i=0;i<currentLevel.noOfEnemies;i++){
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
        //cout<<roundScore<<endl;
    }
    printScore(roundScore);
}

void updateBulletPower(){
    currentLevel.jellyFishPower+=15;
    cout<<"update bullet power"<<endl;
}

bool isCollisionShipWithEnemy(){
    for(int i=0;i<currentLevel.noOfEnemies;i++){
        if(!enemy[i].isAlive)continue;
        float leftX=enemy[i].X4,rightX=enemy[i].X3;
        for(float j=leftX;j<=rightX;j+=2){
            if(j>=ship.shipX5&&j<=ship.shipX2 && enemy[i].Y4<=ship.shipY1){
                float distFromCenter=fabs(ship.shipX1-j),y_positionOfSlog;
                y_positionOfSlog=ship.shipY1-(distFromCenter*2.0);
                if(y_positionOfSlog>=enemy[i].Y4){
                    cout<<"touched... ship and enemy!!"<<endl;
                    return true;
                }
            }
        }
    }
    return false;
}

void vanishJellyFish(int id){
    power_ball[id].y1=-scaleY-100;
    power_ball[id].y2=-scaleY-100;
    power_ball[id].y3=-scaleY-100;
    power_ball[id].y4=-scaleY-100;
    power_ball[id].y5=-scaleY-100;
    power_ball[id].y6=-scaleY-100;
    power_ball[id].y7=-scaleY-100;
    power_ball[id].y8=-scaleY-100;
    power_ball[id].isAlive=false;
}

bool isCollisionShipWithPowerJelly(){
    for(int i=0;i<currentLevel.levelNo;i++){
        if(!power_ball[i].isAlive)continue;
        float leftX=power_ball[i].x6,rightX=power_ball[i].x4;
        for(float j=leftX;j<=rightX;j+=2){
            if(j>=ship.shipX5&&j<=ship.shipX2 && power_ball[i].y6<=ship.shipY1&&power_ball[i].y6>=-scaleX+20){
                float distFromCenter=fabs(ship.shipX1-j),y_positionOfSlog;
                y_positionOfSlog=ship.shipY1-(distFromCenter*2.0);
                if(y_positionOfSlog>=power_ball[i].y6){
                    vanishJellyFish(i);
                    cout<<"touched... ship and jelly fish!!"<<endl;
                    return true;
                }
            }
        }
    }
    return false;
}

void checkCollision(){
    //check collision bullet with enemy
    if(isCollisionShipWithEnemy()){
        isGameOver=true;
        glutIdleFunc(NULL);
        deadText(1);
        return;
    }
    if(isCollisionShipWithPowerJelly()){
        updateBulletPower();
    }
    /*
        enemy co-ordinate
        x1y1    x2y2

        x4y4    x3y3

    */
    int l_score=0;
    for(int i=0;i<10;i++){
        if(bullets[i].isAlive){
            for(int j=0;j<currentLevel.noOfEnemies;j++){
                if(!enemy[j].isAlive)continue;
                for(float k=bullets[i].x-7;k<=bullets[i].x+7;k+=1){
                   if((k >= enemy[j].X4 && k<=enemy[j].X3 )&&
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
    }
    updateScore(l_score);
}

void startMovement(){
    //printf("startMovement\n");
    checkCollision();
    gameOver();
    moveBullet();
    moveEnemy();
    movePower();
}


void startGame(){
    glutIdleFunc(NULL);
    if(isGameOver)return;
    glutIdleFunc(startMovement);
}

void initShipPosition(){
    ship.speed=15;
    ship.shipX1=centerX; ship.shipX2=centerX+50; ship.shipX3=centerX+25; ship.shipX4=centerX-25; ship.shipX5=centerX-50;
    ship.shipY1=centerY+100;ship.shipY2=centerY;ship.shipY3=centerY+25;ship.shipY4=centerY+25;ship.shipY5=centerY;
}

void initLevels(){
    // sazzad
    for(int i=0;i<10;i++){
        levels[i].noOfEnemies=((i+1)*10);
        levels[i].enemySpeed=i*.015+.15;
        levels[i].levelNo=i+1;
        levels[i].power_ball_speed=i*.06 +.20;
    }
    currentLevelIndex=0;
    currentLevel.jellyFishPower=0;
    currentLevel = levels[currentLevelIndex];
    currentLevel.remainingEnemies = currentLevel.noOfEnemies;
}

void init(void){
    //glShadeModel (GL_SMOOTH);
    glClearColor(0.1f, 0.0f, 0.1f, 0.0);
    glColor3f(0.0f, 0.0f, 0.0f);
    glOrtho(-scaleX, scaleX,-scaleY , scaleY, -1.0, 1.0);
    centerX=0;centerY=-scaleY+5;
    isGameOver=false;
    initShipPosition();
    initLevels();
    resetGame();
}

void restartGame(){
    roundScore=0;
    isGameOver=false;
    initShipPosition();
    initLevels();
    resetGame();
}

void charKeyboard(unsigned char key, int x, int y){
    if(isGameOver){
        if(key=='r'||key=='R'){
            cout<<"yes"<<endl;
            restartGame();
            startGame();
        }
        return;
    }
    switch(key){
        case 32:
            loadBullet();
            startGame();
            break;
        case 's':
            cout<<"stop"<<endl;
            glutIdleFunc(NULL);
            break;
        case 'i':
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
    startGame();
    //glutMouseFunc(mouse);
    glutMainLoop();
}

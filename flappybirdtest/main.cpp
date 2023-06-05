#include <windows.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <string>

#define PI 3.1416

using namespace std;

// Global variables
float birdY = 0.0f;
float birdVelocity = -0.0f;
const float GRAVITY = -0.1f;

float pipeX = 100.0f;
float pipeGap = 60.0f;
float pipeWidth = 10.0f;
float pipeHeight = 100.0f;
float pipeSpeed = 1.0f;

int score = 0;
char scoreText[20] = "Score: 0";
bool gameOver = false;

// Window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float CENTER_X = WINDOW_WIDTH / 2;
const float CENTER_Y = WINDOW_HEIGHT / 2;

int pipeCount = 0; // Counter untuk melacak jumlah pipa yang dilewati
int lastPipeCount = -1; // Menyimpan jumlah pipa terakhir yang dilewati

bool checkCollision() {
    // Calculate the bird's bounding box
    float birdLeft = -83.0f;
    float birdRight = -77.0f;
    float birdTop = birdY + 4.0f;
    float birdBottom = birdY - 4.0f;

    // Flag to track if the bird has passed through a pipe
    bool passedThroughPipe = false;

    // Iterate through each pipe
    for (float space = 0; space < 5000.0f; space += 80.0f) {
        // Check if the pipe is within the visible range of the screen
        if (pipeX + space + pipeWidth < -100.0f)
            continue;

        // Calculate the pipe's bounding box
        float pipeLeft = pipeX + space;
        float pipeRight = pipeX + space + pipeWidth;
        float pipeTop = -100.0f + pipeHeight + (pipeGap / 2);
        float pipeBottom = 100.0f - pipeHeight - (pipeGap / 2);

        if (birdRight > pipeLeft && birdLeft < pipeRight) {
            if (birdTop > pipeTop || birdBottom < pipeBottom) {
                // Bird collided with the pipe
                gameOver = true;
                return true;
            } else {
                // Bird passed through the gap

               if (birdTop < pipeTop && birdBottom > pipeBottom && !passedThroughPipe) {
                    // The bird has successfully passed through the space between pipes
                    // Increase the score or perform any other action
                    pipeCount = static_cast<int>(space / 80.0f);
                    if (pipeCount > lastPipeCount) {
                    lastPipeCount = pipeCount;
                    score ++;
                    sprintf(scoreText, "Score: %d", score); // Update teks skor
                    printf("Skor meningkat! Skor = %d\n", score);
                    }
                    passedThroughPipe = true;
               }
            }
        }
    }

    return false; // No collision detected
}

//Input dari keyboard
void keyboard(unsigned char key, int x, int y) {
    if (key == ' ') {
        birdVelocity = 3.0f;
        //jika tekan spasi, maka burungnya akan naik
    }
}

// Update posisi burungnya
void updateBird() {
    birdVelocity += GRAVITY;
    //buat gravitasi, kalau tidak ada inputan akan jatuh
    birdY += birdVelocity;
    //naik turunnya burung akan tergantung birdVelocity

    //batasi pergerakan burung di dalam window
    if (birdY > WINDOW_HEIGHT - 505)
        birdY = WINDOW_HEIGHT - 505;
    else if (birdY < -WINDOW_HEIGHT + 505)
        birdY = -WINDOW_HEIGHT + 505;
}

// Update posisi pipa
void updatePipe(){

    //pipa bergerak dari kanan ke kiri
    pipeX -= pipeSpeed;



}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    //Gambar burung
    float x,y;
    int i;
    GLfloat radius;
    int triangleAmount = 40;
    GLfloat twicePi = 2.0*PI;
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(255, 255, 0);
    x = -80.0f;
    y = birdY;
    radius = 5;
    twicePi = 2.0*PI;
    glVertex2f(x,y); //centre
    for(i = 0; i <= triangleAmount; i++){
        glVertex2f(
                   x+(radius*cos(i*twicePi/triangleAmount)),
                   y+(radius*sin(i*twicePi/triangleAmount)));
    }
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(255, 94, 5);
    glVertex2f(-75.8f,birdY+3.0f); //Q2
    glVertex2f(-72.5f,birdY); //R1
    glVertex2f(-75.8f,birdY-3.0f); //R
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(255, 255, 255);
    x = -78.0f;
    y = birdY+1.9f;
    radius = 1.8;
    twicePi = 2.0*PI;
    glVertex2f(x,y); //centre
    for(i = 0; i <= triangleAmount; i++){
        glVertex2f(
                   x+(radius*cos(i*twicePi/triangleAmount)),
                   y+(radius*sin(i*twicePi/triangleAmount)));
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(0, 0, 0);
    x = -77.0f;
    y = birdY+1.75f;
    radius = 0.95;
    twicePi = 2.0*PI;
    glVertex2f(x,y); //centre
    for(i = 0; i <= triangleAmount; i++){
        glVertex2f(
                   x+(radius*cos(i*twicePi/triangleAmount)),
                   y+(radius*sin(i*twicePi/triangleAmount)));
    }
    glEnd();


    //gambar pipa yang banyak
    for(float space = 0; space < 5000.0f; space+=80.0f){

        float currentPipeHeight = pipeHeight + (rand() % 40 - 20);

        glBegin(GL_POLYGON);
        glColor3ub(0, 255, 0);
        glVertex2f(pipeX + space, -100.0f + pipeHeight + (pipeGap/2));      // Q2
        glVertex2f(pipeX + space, WINDOW_HEIGHT);             // R1
        glVertex2f(pipeX + space + pipeWidth, WINDOW_HEIGHT); // R
        glVertex2f(pipeX + space + pipeWidth, -100.0f + pipeHeight + (pipeGap/2)); // R
        glEnd();

        glBegin(GL_POLYGON);
        glColor3ub(0, 255, 0);
        glVertex2f(pipeX + space, -WINDOW_HEIGHT);                 // Q2
        glVertex2f(pipeX + space, 100.0f - pipeHeight - (pipeGap/2));           // R1
        glVertex2f(pipeX + space + pipeWidth, 100.0f - pipeHeight - (pipeGap/2)); // R
        glVertex2f(pipeX + space + pipeWidth, -WINDOW_HEIGHT);       // R
        glEnd();


    }

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Set the position and color for rendering the score
    glColor3ub(0, 0, 255);  // Adjust color as needed
    glRasterPos2f(10, WINDOW_HEIGHT - 20);  // Adjust position as needed

    sprintf(scoreText, "Score: %d", score);

    // Render each character of the score string
    for (int i = 0; scoreText[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, scoreText[i]);
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glutSwapBuffers();
}

//Masukkan timer function untuk update komponen dan gambar ulang scenenya
void update(int value) {

    if(!gameOver){
        updateBird();
        updatePipe();
    }


    if (checkCollision()) {
        // Handle collision, such as ending the game or resetting the game
        printf("Game Over\n");
        // Add your code here to handle the collision
    }

    //gambar ulang scene seakan akan animasinya bergerak
    glutPostRedisplay();

    //Set jarak update selanjutnya
    glutTimerFunc(16, update, 0);
}

void init() {
    glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
    glLoadIdentity();
    glOrtho(-100.0, 100.0, -100.0, 100.0, -1.0, 1.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Flappy Bird");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, update, 0);
    init();
    glutMainLoop();
    return 0;
}

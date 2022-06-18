
// gọi các thư viện
#include <GL/glut.h>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <mmsystem.h>
#include "imageloader.h"
using namespace std;

// Khởi tạo các giá trị
struct block {
    // vị trí của khối
    int pos_i, pos_j, pos_k;
    //6 mặt Rubik
    int top, bottom, left, right, front, back;
};

// kích thước
#define SIZE 3
#define CUBESIZE 1
block cube[SIZE][SIZE][SIZE];

// xoay khối Rubik
GLint rot_x = 0, rot_y = 0;
float angle, speed = 0.0f, speedrange = 0.3f;
int rotationcomplete = 0, rotation = 0, inverse = 0, beginx = 0, beginy = 0, moving = 0, reflect = 0, playsound = 0, skybox = 200, speedmetercount = 0;
GLfloat p = 0.0, q = 0.0;
int speedmetercolor[4] = { 0,0,0,0};
std::vector<int> solve;
//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image) {
    GLuint textureId;
    // tạo 1 texture, tăng số lên nếu bạn muốn load nhiều hơn 1 texture
    glGenTextures(1, &textureId); 
    // gán id
    glBindTexture(GL_TEXTURE_2D, textureId);
    //bộ lọc
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
    return textureId;
}


//thiết lập lại vị trí của các khối cube(nhỏ) để đổi vị trí  
void rotate90(int& x, int& y)
{
    x -= 1;
    y -= 1;
    int temp = x;
    x = -y;
    y = temp;
    x += 1;
    y += 1;
}

//thiết lập lại vị trí của các khối cube(nhỏ) để đổi vị trí 
void rotate_minus_90(int& x, int& y)
{
    x -= 1;
    y -= 1;
    int temp = x;
    x = y;
    y = -temp;
    x += 1;
    y += 1;
}

//xoay mặt trái theo chiều kim đồng hồ (nhấn phím a)
void rotate_left_clockwise()
{

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            for (int k = 0; k < SIZE; k++)
            {
                block& t = cube[i][j][k];
                if (t.pos_i == 0) // tất cả các cube có vị tri pos_i = 0 sẽ xoay
                {
                    // thiết lập lại vị trí các khối cube
                    rotate_minus_90(t.pos_j, t.pos_k);
                    //đổi màu các mặt
                    int temp = t.top;
                    t.top = t.front;
                    t.front = t.bottom;
                    t.bottom = t.back;
                    t.back = temp;
                }

            }
}
//xoay mặt trái ngược chiều kim đồng hồ (nhấn phím s)
void rotate_left_anticlockwise()
{

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            for (int k = 0; k < SIZE; k++)
            {
                block& t = cube[i][j][k];
                if (t.pos_i == 0) // tất cả các cube có vị tri pos_i = 0 sẽ xoay
                {
                    //thiết lập lại vị trí các khối cube
                    rotate90(t.pos_j, t.pos_k);
                    //đổi màu các mặt
                    int temp = t.top;
                    t.top = t.back;
                    t.back = t.bottom;
                    t.bottom = t.front;
                    t.front = temp;
                }

            }


}

//xoay mặt phải ngược chiều kim đồng hồ (nhấn phím f)
void rotate_right_anticlockwise()
{

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            for (int k = 0; k < SIZE; k++)
            {
                block& t = cube[i][j][k];
                if (t.pos_i == 2) // tất cả các cube có vị tri pos_i = 2 sẽ xoay
                {
                    //thiết lập lại vị trí các khối cube
                    rotate90(t.pos_j, t.pos_k);
                    //đổi màu các mặt
                    int temp = t.top;
                    t.top = t.back;
                    t.back = t.bottom;
                    t.bottom = t.front;
                    t.front = temp;
                }

            }


}




//xoay mặt phải theo chiều kim đồng hồ (nhấn phím d)
void rotate_right_clockwise()
{

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            for (int k = 0; k < SIZE; k++)
            {
                block& t = cube[i][j][k];
                if (t.pos_i == 2) // tất cả các cube có vị tri pos_i = 2 sẽ xoay
                {
                    //thiết lập lại vị trí các khối cube
                    rotate_minus_90(t.pos_j, t.pos_k);
                    //đổi màu các mặt
                    int temp = t.top;
                    t.top = t.front;
                    t.front = t.bottom;
                    t.bottom = t.back;
                    t.back = temp;
                }

            }


}



//xoay mặt trước theo chiều kim đồng hồ (nhấn phím z)
void rotate_front_clockwise()
{

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            for (int k = 0; k < SIZE; k++)
            {
                block& t = cube[i][j][k];
                if (t.pos_k == 2)
                {
                    rotate_minus_90(t.pos_i, t.pos_j);
                    int temp = t.top;
                    t.top = t.left;
                    t.left = t.bottom;
                    t.bottom = t.right;
                    t.right = temp;
                }

            }


}



//xoay mặt trước ngược chiều kim đồng hồ (nhấn phím x)
void rotate_front_anticlockwise()
{

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            for (int k = 0; k < SIZE; k++)
            {
                block& t = cube[i][j][k];
                if (t.pos_k == 2)
                {
                    rotate90(t.pos_i, t.pos_j);
                    int temp = t.top;
                    t.top = t.right;
                    t.right = t.bottom;
                    t.bottom = t.left;
                    t.left = temp;
                }

            }


}




//xoay mặt sau theo chiều kim đồng hồ (nhấn phím c)
void rotate_back_clockwise()
{

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            for (int k = 0; k < SIZE; k++)
            {
                block& t = cube[i][j][k];
                if (t.pos_k == 0)
                {
                    rotate_minus_90(t.pos_i, t.pos_j);
                    int temp = t.top;
                    t.top = t.left;
                    t.left = t.bottom;
                    t.bottom = t.right;
                    t.right = temp;
                }

            }


}




//xoay mặt sau ngược chiều kim đồng hồ (nhấn phím v)
void rotate_back_anticlockwise()
{

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            for (int k = 0; k < SIZE; k++)
            {
                block& t = cube[i][j][k];
                if (t.pos_k == 0)
                {
                    rotate90(t.pos_i, t.pos_j);
                    int temp = t.top;
                    t.top = t.right;
                    t.right = t.bottom;
                    t.bottom = t.left;
                    t.left = temp;
                }

            }


}



//xoay mặt trên ngược chiều kim đồng hồ (nhấn phím w)
void rotate_top_anticlockwise()
{

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            for (int k = 0; k < SIZE; k++)
            {
                block& t = cube[i][j][k];
                if (t.pos_j == 2)
                {
                    rotate_minus_90(t.pos_i, t.pos_k);
                    int temp = t.back;
                    t.back = t.right;
                    t.right = t.front;
                    t.front = t.left;
                    t.left = temp;
                }
            }
}



//xoay mặt trên theo chiều kim đồng hồ (nhấn phím q)
void rotate_top_clockwise()
{

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            for (int k = 0; k < SIZE; k++)
            {
                block& t = cube[i][j][k];
                if (t.pos_j == 2)
                {
                    rotate90(t.pos_i, t.pos_k);
                    int temp = t.back;
                    t.back = t.left;
                    t.left = t.front;
                    t.front = t.right;
                    t.right = temp;
                }

            }


}


//xoay mặt dưới ngược chiều kim đồng hồ (nhấn phím r)
void rotate_bottom_anticlockwise()
{

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            for (int k = 0; k < SIZE; k++)
            {
                block& t = cube[i][j][k];
                if (t.pos_j == 0)
                {
                    rotate_minus_90(t.pos_i, t.pos_k);
                    int temp = t.back;
                    t.back = t.right;
                    t.right = t.front;
                    t.front = t.left;
                    t.left = temp;
                }

            }


}

//xoay mặt dưới theo chiều kim đồng hồ (nhấn phím e)
void rotate_bottom_clockwise()
{
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            for (int k = 0; k < SIZE; k++)
            {
                block& t = cube[i][j][k];
                if (t.pos_j == 0)
                {
                    rotate90(t.pos_i, t.pos_k);
                    int temp = t.back;
                    t.back = t.left;
                    t.left = t.front;
                    t.front = t.right;
                    t.right = temp;
                }

            }

}
//thiết lập ma trận lưu trữ các điểm vẽ của khối cube(tạo điểm để vẽ)
//6 mặt, mỗi mặt 4 điểm, vị trí xyz mỗi điểm
float cube_vertexes[6][4][3] =
{
    // mặt trái
  {
    {-CUBESIZE, -CUBESIZE, -CUBESIZE},
    {-CUBESIZE, -CUBESIZE, CUBESIZE},
    {-CUBESIZE, CUBESIZE, CUBESIZE},
    {-CUBESIZE, CUBESIZE, -CUBESIZE}},
    // mặt phải
  {
    {CUBESIZE, CUBESIZE, CUBESIZE},
    {CUBESIZE, -CUBESIZE, CUBESIZE},
    {CUBESIZE, -CUBESIZE, -CUBESIZE},
    {CUBESIZE, CUBESIZE, -CUBESIZE}},
    // mặt dưới
  {
    {-CUBESIZE, -CUBESIZE, -CUBESIZE},
    {CUBESIZE, -CUBESIZE, -CUBESIZE},
    {CUBESIZE, -CUBESIZE, CUBESIZE},
    {-CUBESIZE, -CUBESIZE, CUBESIZE}},
    // mặt trên
  {
    {CUBESIZE, CUBESIZE, CUBESIZE},
    {CUBESIZE, CUBESIZE, -CUBESIZE},
    {-CUBESIZE, CUBESIZE, -CUBESIZE},
    {-CUBESIZE, CUBESIZE, CUBESIZE}},
    // mặt sau
  {
    {-CUBESIZE, -CUBESIZE, -CUBESIZE},
    {-CUBESIZE, CUBESIZE, -CUBESIZE},
    {CUBESIZE, CUBESIZE, -CUBESIZE},
    {CUBESIZE, -CUBESIZE, -CUBESIZE}},
    // mặt trước
  {
    {CUBESIZE, CUBESIZE, CUBESIZE},
    {-CUBESIZE, CUBESIZE, CUBESIZE},
    {-CUBESIZE, -CUBESIZE, CUBESIZE},
    {CUBESIZE, -CUBESIZE, CUBESIZE}}
};

float color[5][3] =
{
    {1.0,1.0,1.0},
    {0.0,1.0,0.0}
};

float cube_normals[6][3] =
{
  {-1.0, 0.0, 0.0},
  {1.0, 0.0, 0.0},
  {0.0, -1.0, 0.0},
  {0.0, 1.0, 0.0},
  {0.0, 0.0, -1.0},
  {0.0, 0.0, 1.0}
};

//vẽ  khối cube
void draw_cube(int x, int y, int z)
{
    //vẽ khối
    glPushMatrix();
    //tạo biến ma trận t (lưu vị trí)
    block t = cube[x][y][z];
    int texture[] = { t.left, t.right, t.bottom ,t.top, t.back, t.front };
    glTranslatef((CUBESIZE * 2) * t.pos_i, (CUBESIZE * 2) * t.pos_j, (CUBESIZE * 2) * t.pos_k);
    glEnable(GL_TEXTURE_2D);
    for (int i = 0; i < 6; i++) {
        glBindTexture(GL_TEXTURE_2D, texture[i]);
        glBegin(GL_QUADS);
        //glNormal3fv(&cube_normals[i][0]);
        glTexCoord2f(0.0, 0.0);
        glVertex3fv(&cube_vertexes[i][0][0]);
        glTexCoord2f(0.0, 1.0);
        glVertex3fv(&cube_vertexes[i][1][0]);
        glTexCoord2f(1.0, 1.0);
        glVertex3fv(&cube_vertexes[i][2][0]);
        glTexCoord2f(1.0, 0.0);
        glVertex3fv(&cube_vertexes[i][3][0]);
        glEnd();
    }

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

}

// vẽ background
void drawbackground() {
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 8);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-skybox, -skybox, -skybox);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-skybox, skybox, -skybox);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(skybox, skybox, -skybox);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(skybox, -skybox, -skybox);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 9);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-skybox, -skybox, -skybox);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(skybox, -skybox, -skybox);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(skybox, -skybox, skybox);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-skybox, -skybox, skybox);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 10);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-skybox, -skybox, skybox);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-skybox, skybox, skybox);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(skybox, skybox, skybox);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(skybox, -skybox, skybox);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 11);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-skybox, -skybox, -skybox);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-skybox, skybox, -skybox);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-skybox, skybox, skybox);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-skybox, -skybox, skybox);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 12);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(skybox, -skybox, -skybox);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(skybox, skybox, -skybox);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(skybox, skybox, skybox);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(skybox, -skybox, skybox);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 13);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-skybox, skybox, -skybox);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(skybox, skybox, -skybox);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(skybox, skybox, skybox);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-skybox, skybox, skybox);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}
void drawFloor()
{
    glDisable(GL_LIGHTING);
    glBegin(GL_QUADS);
    glVertex3f(-20, -SIZE * 2 * CUBESIZE, 20);
    glVertex3f(20, -SIZE * 2 * CUBESIZE, 20);
    glVertex3f(20, -SIZE * 2 * CUBESIZE, -20);
    glVertex3f(-20, -SIZE * 2 * CUBESIZE, -20);
    glEnd();
    glEnable(GL_LIGHTING);
}

void drawrubik() {
    glDisable(GL_LIGHTING);
    drawbackground();
    glEnable(GL_LIGHTING);
    if (rotation == 0)
    {

        glTranslatef(-(float)((CUBESIZE * 2) / 2 * (SIZE - 1)), -(float)((CUBESIZE * 2) / 2 * (SIZE - 1)), -(float)((CUBESIZE * 2) / 2 * (SIZE - 1)));
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                for (int k = 0; k < SIZE; k++) {
                    draw_cube(i, j, k);
                }

    }
    if (rotation == 1) {

        glPushMatrix();
        glTranslatef(-(float)((CUBESIZE * 2) / 2 * (SIZE - 1)), -(float)((CUBESIZE * 2) / 2 * (SIZE - 1)), -(float)((CUBESIZE * 2) / 2 * (SIZE - 1)));
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                for (int k = 0; k < SIZE; k++) {
                    block& t = cube[i][j][k];
                    if (t.pos_j == 0 || t.pos_j == 1) {
                        draw_cube(i, j, k);
                    }
                }
        glPopMatrix();
        glPushMatrix();
        if (inverse == 0)
        {
            glRotatef(-angle, 0.0, 1.0, 0.0);

        }
        else
        {
            glRotatef(angle, 0.0, 1.0, 0.0);
        }
        glTranslatef(-(float)((CUBESIZE * 2) / 2 * (SIZE - 1)), -(float)((CUBESIZE * 2) / 2 * (SIZE - 1)), -(float)((CUBESIZE * 2) / 2 * (SIZE - 1)));
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                for (int k = 0; k < SIZE; k++) {
                    block& t = cube[i][j][k];
                    if (t.pos_j == 2) {
                        draw_cube(i, j, k);
                    }
                }
        glPopMatrix();
    }

    if (rotation == 2) {
        glPushMatrix();
        glTranslatef(-(float)((CUBESIZE * 2) / 2 * (SIZE - 1)), -(float)((CUBESIZE * 2) / 2 * (SIZE - 1)), -(float)((CUBESIZE * 2) / 2 * (SIZE - 1)));
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                for (int k = 0; k < SIZE; k++) {
                    block& t = cube[i][j][k];
                    if (t.pos_j == 1 || t.pos_j == 2) {
                        draw_cube(i, j, k);
                    }
                }
        glPopMatrix();
        glPushMatrix();
        if (inverse == 0)
        {
            glRotatef(-angle, 0.0, 1.0, 0.0);

        }
        else
        {
            glRotatef(angle, 0.0, 1.0, 0.0);
        }
        glTranslatef(-(float)((CUBESIZE * 2) / 2 * (SIZE - 1)), -(float)((CUBESIZE * 2) / 2 * (SIZE - 1)), -(float)((CUBESIZE * 2) / 2 * (SIZE - 1)));
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                for (int k = 0; k < SIZE; k++) {
                    block& t = cube[i][j][k];
                    if (t.pos_j == 0) {
                        draw_cube(i, j, k);
                    }
                }
        glPopMatrix();
    }

    if (rotation == 3) {
        glPushMatrix();
        glTranslatef(-(float)((CUBESIZE * 2) / 2 * (SIZE - 1)), -(float)((CUBESIZE * 2) / 2 * (SIZE - 1)), -(float)((CUBESIZE * 2) / 2 * (SIZE - 1)));
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                for (int k = 0; k < SIZE; k++) {
                    block& t = cube[i][j][k];
                    if (t.pos_i == 1 || t.pos_i == 2) {
                        draw_cube(i, j, k);
                    }
                }
        glPopMatrix();
        glPushMatrix();
        if (inverse == 0)
        {
            glRotatef(-angle, 1.0, 0.0, 0.0);

        }
        else
        {
            glRotatef(angle, 1.0, 0.0, 0.0);
        }
        glTranslatef(-(float)((CUBESIZE * 2) / 2 * (SIZE - 1)), -(float)((CUBESIZE * 2) / 2 * (SIZE - 1)), -(float)((CUBESIZE * 2) / 2 * (SIZE - 1)));
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                for (int k = 0; k < SIZE; k++) {
                    block& t = cube[i][j][k];
                    if (t.pos_i == 0) {
                        draw_cube(i, j, k);
                    }
                }
        glPopMatrix();
    }

    if (rotation == 4) {
        glPushMatrix();
        glTranslatef(-(float)((CUBESIZE * 2) / 2 * (SIZE - 1)), -(float)((CUBESIZE * 2) / 2 * (SIZE - 1)), -(float)((CUBESIZE * 2) / 2 * (SIZE - 1)));
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                for (int k = 0; k < SIZE; k++) {
                    block& t = cube[i][j][k];
                    if (t.pos_i == 0 || t.pos_i == 1) {
                        draw_cube(i, j, k);
                    }
                }
        glPopMatrix();
        glPushMatrix();
        if (inverse == 0)
        {
            glRotatef(-angle, 1.0, 0.0, 0.0);

        }
        else
        {
            glRotatef(angle, 1.0, 0.0, 0.0);
        }
        glTranslatef(-(float)((CUBESIZE * 2) / 2 * (SIZE - 1)), -(float)((CUBESIZE * 2) / 2 * (SIZE - 1)), -(float)((CUBESIZE * 2) / 2 * (SIZE - 1)));
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                for (int k = 0; k < SIZE; k++) {
                    block& t = cube[i][j][k];
                    if (t.pos_i == 2) {
                        draw_cube(i, j, k);
                    }
                }
        glPopMatrix();
    }

    if (rotation == 5) {
        glPushMatrix();
        glTranslatef(-(float)((CUBESIZE * 2) / 2 * (SIZE - 1)), -(float)((CUBESIZE * 2) / 2 * (SIZE - 1)), -(float)((CUBESIZE * 2) / 2 * (SIZE - 1)));
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                for (int k = 0; k < SIZE; k++) {
                    block& t = cube[i][j][k];
                    if (t.pos_k == 0 || t.pos_k == 1) {
                        draw_cube(i, j, k);
                    }
                }
        glPopMatrix();
        glPushMatrix();
        if (inverse == 0)
        {
            glRotatef(-angle, 0.0, 0.0, 1.0);

        }
        else
        {
            glRotatef(angle, 0.0, 0.0, 1.0);
        }
        glTranslatef(-(float)((CUBESIZE * 2) / 2 * (SIZE - 1)), -(float)((CUBESIZE * 2) / 2 * (SIZE - 1)), -(float)((CUBESIZE * 2) / 2 * (SIZE - 1)));
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                for (int k = 0; k < SIZE; k++) {
                    block& t = cube[i][j][k];
                    if (t.pos_k == 2) {
                        draw_cube(i, j, k);
                    }
                }
        glPopMatrix();
    }

    if (rotation == 6) {
        glPushMatrix();
        glTranslatef(-(float)((CUBESIZE * 2) / 2 * (SIZE - 1)), -(float)((CUBESIZE * 2) / 2 * (SIZE - 1)), -(float)((CUBESIZE * 2) / 2 * (SIZE - 1)));
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                for (int k = 0; k < SIZE; k++) {
                    block& t = cube[i][j][k];
                    if (t.pos_k == 1 || t.pos_k == 2) {
                        draw_cube(i, j, k);
                    }
                }
        glPopMatrix();
        glPushMatrix();
        if (inverse == 0)
        {
            glRotatef(-angle, 0.0, 0.0, 1.0);

        }
        else
        {
            glRotatef(angle, 0.0, 0.0, 1.0);
        }
        glTranslatef(-(float)((CUBESIZE * 2) / 2 * (SIZE - 1)), -(float)((CUBESIZE * 2) / 2 * (SIZE - 1)), -(float)((CUBESIZE * 2) / 2 * (SIZE - 1)));
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                for (int k = 0; k < SIZE; k++) {
                    block& t = cube[i][j][k];
                    if (t.pos_k == 0) {
                        draw_cube(i, j, k);
                    }
                }
        glPopMatrix();
    }
}

void drawreflection() {
    // tắt chế độ hiển thị màu và độ sâu
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_COLOR);
    //bật chế độ vẽ vào bộ đệm stencil. 
    glEnable(GL_STENCIL_TEST);
    // cho phép thay thế đối tượng
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 1, 1);

    //Bây giờ vẽ sàn chỉ cần gắn thẻ pixel của sàn là giá trị stencil 1.
    drawFloor();

    //Bật lại cập nhật màu sắc và độ sâu.
    glEnable(GL_COLOR);
    glEnable(GL_DEPTH_TEST);

    //giữ lại các đối tượng, hiệu ứng được thêm bổ sung
    glStencilFunc(GL_EQUAL, 1, 1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    //Vẽ rubik phản chiếu, nhưng chỉ nơi sàn.
    glPushMatrix();
    glTranslatef(0, - SIZE * 2 * CUBESIZE * 2, 0);
    glScalef(1.0, -1.0, 1.0);
    drawrubik();
    glPopMatrix();

    glDisable(GL_STENCIL_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1, 1, 1, 0.0); 
    drawFloor();
    glDisable(GL_BLEND);
}

void setcolor(int p){
    glColor3fv(color[p]);
}

void speedmeter()
{
    glDisable(GL_LIGHTING);

    glPushMatrix();
    glTranslatef(1.0, 0.0, 0.0);
    glColor3f(0, 0, 0);
    glLineWidth(3.0);
    glBegin(GL_LINE_LOOP);
    glVertex3f(0,10,0);
    glVertex3f(1, 10, 0);
    glVertex3f(1, 9, 0);
    glVertex3f(0, 9, 0);
    glEnd();

    setcolor(speedmetercolor[0]);
    glBegin(GL_QUADS);
    glVertex3f(0, 10, 0);
    glVertex3f(1, 10, 0);
    glVertex3f(1, 9, 0);
    glVertex3f(0, 9, 0);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2.5, 0.0, 0.0);
    glColor3f(0, 0, 0);
    glLineWidth(3.0);
    glBegin(GL_LINE_LOOP);
    glVertex3f(0, 10, 0);
    glVertex3f(1, 10, 0);
    glVertex3f(1, 9, 0);
    glVertex3f(0, 9, 0);
    glEnd();

    setcolor(speedmetercolor[1]);
    glBegin(GL_QUADS);
    glVertex3f(0, 10, 0);
    glVertex3f(1, 10, 0);
    glVertex3f(1, 9, 0);
    glVertex3f(0, 9, 0);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(4, 0.0, 0.0);
    glColor3f(0, 0, 0);
    glLineWidth(3.0);
    glBegin(GL_LINE_LOOP);
    glVertex3f(0, 10, 0);
    glVertex3f(1, 10, 0);
    glVertex3f(1, 9, 0);
    glVertex3f(0, 9, 0);
    glEnd();

    setcolor(speedmetercolor[2]);
    glBegin(GL_QUADS);
    glVertex3f(0, 10, 0);
    glVertex3f(1, 10, 0);
    glVertex3f(1, 9, 0);
    glVertex3f(0, 9, 0);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(5.5, 0.0, 0.0);
    glColor3f(0, 0, 0);
    glLineWidth(3.0);
    glBegin(GL_LINE_LOOP);
    glVertex3f(0, 10, 0);
    glVertex3f(1, 10, 0);
    glVertex3f(1, 9, 0);
    glVertex3f(0, 9, 0);
    glEnd();

    setcolor(speedmetercolor[3]);
    glBegin(GL_QUADS);
    glVertex3f(0, 10, 0);
    glVertex3f(1, 10, 0);
    glVertex3f(1, 9, 0);
    glVertex3f(0, 9, 0);
    glEnd();
    glPopMatrix();

    glEnable(GL_LIGHTING);

}

// hiển thị 
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glLoadIdentity();
    //điểm nhìn
    gluLookAt(0, 0, 35, 0, 0, 0, 0, 1, 0);
    speedmeter();
    glColor3f(1.0, 1.0, 1.0);
    //xoay quanh x
    glRotatef(rot_x, 1.0, 0.0, 0.0);
    //xoay quanh y
    glRotatef(rot_y, 0.0, 1.0, 0.0);
    //vẽ các khối cude

    glPushMatrix();
    glRotatef(p, 1.0, 0.0, 0.0);
    glRotatef(q, 0.0, 1.0, 0.0);

    if (reflect == 1)
        drawreflection();
    drawrubik();
    
    glPopMatrix();
    //thay đổi hiển thị
    glutSwapBuffers();
}

//định dạng khối cube
void initcube() {

    loadTexture(loadBMP("white.bmp"));
    loadTexture(loadBMP("red.bmp"));
    loadTexture(loadBMP("blue.bmp"));
    loadTexture(loadBMP("orange.bmp"));
    loadTexture(loadBMP("green.bmp"));
    loadTexture(loadBMP("yellow.bmp"));
    loadTexture(loadBMP("red1.bmp"));
    loadTexture(loadBMP("back.bmp"));
    loadTexture(loadBMP("bottom.bmp"));
    loadTexture(loadBMP("front.bmp"));
    loadTexture(loadBMP("right.bmp"));
    loadTexture(loadBMP("left.bmp"));
    loadTexture(loadBMP("top.bmp"));

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            for (int k = 0; k < SIZE; k++)
            {
                block& t = cube[i][j][k];
                t.left = t.right = t.bottom = t.top = t.back = t.front = 7;
            }
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++) {
            cube[0][i][j].left = 3;
            cube[SIZE - 1][i][j].right = 5;
            cube[i][0][j].bottom = 6;
            cube[i][SIZE - 1][j].top = 1;
            cube[i][j][0].back = 4;
            cube[i][j][SIZE - 1].front = 2;

        }
    //thiết lập vị trí các khối
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)  
            for (int k = 0; k < SIZE; k++) {
                cube[i][j][k].pos_i = SIZE + i - SIZE;
                cube[i][j][k].pos_j = SIZE + j - SIZE;
                cube[i][j][k].pos_k = SIZE + k - SIZE;
            }
}

//thiết lập chiếu sáng
void InitLighting() {
    //Chiếu sáng toàn phần
    GLfloat ambient_light[] = { 2,2,2,1.0 };
    GLfloat ambient_light1[] = { 1,1,1,1.0 };

    // ánh sáng phản xạ
    GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };

    //điểm chiếu sáng
    GLfloat light_position[] = { -25, 20, -30, 0 };
    GLfloat light_position1[] = { 0, -10, 0, 0 };
    //truyền tham số
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient_light1);

    //Cho phép chiếu sáng
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

//Khung chương trình
void Init()
{
    InitLighting();
    //cho phép hiển thị theo độ sâu
    glEnable(GL_DEPTH_TEST);
    initcube();

}
//thiết lập góc nhìn
void reshape_func(int w, int h)
{
    glViewport(0, 0, w, h);
    float ratio = (float)w / (float)h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, ratio, 1.0, 450);
    glMatrixMode(GL_MODELVIEW);
}
void spincube()
{
    angle += 0.5 + speed;
    if (angle >= 90)
    {
        rotationcomplete = 1;
        glutIdleFunc(NULL);
        if (rotation == 1 && inverse == 0)
        {
            rotate_top_clockwise();
        }
        if (rotation == 1 && inverse == 1)
        {
            rotate_top_anticlockwise();
        }
        if (rotation == 2 && inverse == 0)
        {
            rotate_bottom_clockwise();
        }
        if (rotation == 2 && inverse == 1)
        {
            rotate_bottom_anticlockwise();
        }
        if (rotation == 3 && inverse == 0)
        {
            rotate_left_clockwise();
        }
        if (rotation == 3 && inverse == 1)
        {
            rotate_left_anticlockwise();
        }
        if (rotation == 4 && inverse == 0)
        {
            rotate_right_clockwise();
        }
        if (rotation == 4 && inverse == 1)
        {
            rotate_right_anticlockwise();
        }
        if (rotation == 5 && inverse == 0)
        {
            rotate_front_clockwise();
        }
        if (rotation == 5 && inverse == 1)
        {
            rotate_front_anticlockwise();
        }
        if (rotation == 6 && inverse == 0)
        {
            rotate_back_clockwise();
        }
        if (rotation == 6 && inverse == 1)
        {
            rotate_back_anticlockwise();
        }
        rotation = 0;
        angle = 0;
    }
    glutPostRedisplay();
}

void motion(int x, int y)
{
    if (moving)
    {
        q = q + (x - beginx);
        beginx = x;
        p = p + (y - beginy);
        beginy = y;
        glutPostRedisplay();
    }
}


void mouse(int btn, int state, int x, int y)
{
    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        moving = 1;
        beginx = x;
        beginy = y;
    }
}

// sự kiện nhấn các phím
void keyboard_func(unsigned char key, int x, int y)
{

    switch (key) {
        // thiết lập xoay đối tượng
    case 'l': // Phải
        rot_y = (rot_y - 5) % 360;
        break;

    case 'j': // Trái
        rot_y = (rot_y + 5) % 360;
        break;

    case 'i': // Xuống
        rot_x = (rot_x + 5) % 360;
        break;

    case 'k': // Lên
        rot_x = (rot_x - 5) % 360;
        break;

        //các phím điều khiển
    case 'q':
        if (rotationcomplete == 1) {
            rotationcomplete = 0;
            rotation = 1;
            inverse = 0;
            solve.push_back(1);
            glutIdleFunc(spincube);
        }
        break;

    case 'w':
        if (rotationcomplete == 1) {
            rotationcomplete = 0;
            rotation = 1;
            inverse = 1;
            solve.push_back(-1);
            glutIdleFunc(spincube);
        }
        break;

    case 'e':
        if (rotationcomplete == 1) {
            rotationcomplete = 0;
            rotation = 2;
            inverse = 0;
            solve.push_back(2);
            glutIdleFunc(spincube);
        }
        break;

    case 'r':
        if (rotationcomplete == 1) {
            rotationcomplete = 0;
            rotation = 2;
            inverse = 1;
            solve.push_back(-2);
            glutIdleFunc(spincube);
        }
        break;

    case 'a':
        if (rotationcomplete == 1) {
            rotationcomplete = 0;
            rotation = 3;
            inverse = 0;
            solve.push_back(3);
            glutIdleFunc(spincube);
        }
        break;

    case 's':
        if (rotationcomplete == 1) {
            rotationcomplete = 0;
            rotation = 3;
            inverse = 1;
            solve.push_back(-3);
            glutIdleFunc(spincube);
        }
        break;

    case 'd':
        if (rotationcomplete == 1) {
            rotationcomplete = 0;
            rotation = 4;
            inverse = 0;
            solve.push_back(4);
            glutIdleFunc(spincube);
        }
        break;

    case 'f':
        if (rotationcomplete == 1) {
            rotationcomplete = 0;
            rotation = 4;
            inverse = 1;
            solve.push_back(-4);
            glutIdleFunc(spincube);
        }
        break;

    case 'z':
        if (rotationcomplete == 1) {
            rotationcomplete = 0;
            rotation = 5;
            inverse = 0;
            solve.push_back(5);
            glutIdleFunc(spincube);
        }
        break;

    case 'x':
        if (rotationcomplete == 1) {
            rotationcomplete = 0;
            rotation = 5;
            inverse = 1;
            solve.push_back(-5);
            glutIdleFunc(spincube);
        }
        break;

    case 'c':
        if (rotationcomplete == 1) {
            rotationcomplete = 0;
            rotation = 6;
            inverse = 0;
            solve.push_back(6);
            glutIdleFunc(spincube);
        }
        break;

    case 'v':
        if (rotationcomplete == 1) {
            rotationcomplete = 0;
            rotation = 6;
            inverse = 1;
            solve.push_back(-6);
            glutIdleFunc(spincube);
        }
        break;

    case 'm':
        if (speed < speedrange*4.0f) {
            speed += speedrange;
            speedmetercolor[speedmetercount] = 1;
            speedmetercount++;
        }
        break;

    case 'n': 
        if (speed >= speedrange)
        {
            speed -= speedrange;
            speedmetercount--;
            speedmetercolor[speedmetercount] = 0;
        }
        break;

    case 'o':
        if (rotationcomplete == 1) {
            rotationcomplete = 0;
            if (solve.size() > 0)
            {
                if (solve.back() < 0)
                {
                    rotation = -1 * solve.back();
                    inverse = 0;
                }
                if (solve.back() > 0)
                {
                    rotation = solve.back();
                    inverse = 1;
                }
                solve.pop_back();
            }
            glutIdleFunc(spincube);
        }
        break;

    case 't':
        if (reflect != 1) {
            reflect = 1;
            glEnable(GL_LIGHT1);
        }
        else {
            reflect = 0;
            glDisable(GL_LIGHT1);
        }
        break;

    case 'b':
        if (playsound != 1) {
            playsound = 1;
            PlaySound(NULL, NULL, SND_ASYNC);
        }
        else
        {
            playsound = 0;
            PlaySound(TEXT("file_example_WAV_10MG.wav"), NULL, SND_ASYNC | SND_LOOP);
        }
        break;
    default:
        break;

    }
    glutPostRedisplay();

}

//hàm main
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Rubik 3D");
    PlaySound(TEXT("file_example_WAV_10MG.wav"), NULL, SND_ASYNC | SND_LOOP);
    glutDisplayFunc(display);
    glutIdleFunc(spincube);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutReshapeFunc(reshape_func);
    Init();
    glutKeyboardFunc(keyboard_func);
    glutMainLoop();
}


#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<Windows.h>
#include<GL/glut.h>

double cameraAngle;
double move_X, move_Y, move_Z;
int canDrawGrid, canDrawAxis;

double cameraRadius, cameraHeight, cameraAngleDelta;
int side;
int points;
int pw_slow;
float px, py;
float gx_1, gy_1, gx_2, gy_2, gx_3, gy_3;
bool r_m_s[25];
int mat_range = 201;
char game_map[201][201];
int game_lvl;
int pacman_life;
bool halt_game;

void drawGridAndAxes() {
    // draw the three major AXES

    glBegin(GL_LINES);
    //X axis
    glColor3f(0, 1, 0); //100% Green
    glVertex3f(-150, 0, 0);
    glVertex3f(150, 0, 0);

    //Y axis
    glColor3f(0, 0, 1); //100% Blue
    glVertex3f(0, -150, 0); // intentionally extended to -150 to 150, no big deal
    glVertex3f(0, 150, 0);

    //Z axis
    glColor3f(1, 1, 1); //100% White
    glVertex3f(0, 0, -150);
    glVertex3f(0, 0, 150);
    glEnd();

    //some gridlines along the field
    int i;

    glColor3f(0.5, 0.5, 0.5); //grey
    glBegin(GL_LINES);
    for (i = -10; i <= 10; i++) {
        if (i == 0)
            continue; //SKIP the MAIN axes

        //lines parallel to Y-axis
        glVertex3f(i * 10, -100, 0);
        glVertex3f(i * 10, 100, 0);

        //lines parallel to X-axis
        glVertex3f(-100, i * 10, 0);
        glVertex3f(100, i * 10, 0);
    }
    glEnd();
}

void borders() {
    //front a
    glPushMatrix();
    glColor3f(.5, .5, .5);
    glTranslatef(0, -100, 2.5);
    glScalef(200, 5, 5);
    glutSolidCube(1);
    glPopMatrix();
    //back b
    glPushMatrix();
    glColor3f(.5, .5, .5);
    glTranslatef(0, 100, 2.5);
    glScalef(200, 5, 5);
    glutSolidCube(1);
    glPopMatrix();
    //right_up c
    glPushMatrix();
    glColor3f(.5, .5, .5);
    glTranslatef(100, 55, 2.5);
    glScalef(5, 90, 5);
    glutSolidCube(1);
    glPopMatrix();
    //left_up d
    glPushMatrix();
    glColor3f(.5, .5, .5);
    glTranslatef(-100, 55, 2.5);
    glScalef(5, 90, 5);
    glutSolidCube(1);
    glPopMatrix();
    //right_low e
    glPushMatrix();
    glColor3f(.5, .5, .5);
    glTranslatef(100, -55, 2.5);
    glScalef(5, 90, 5);
    glutSolidCube(1);
    glPopMatrix();
    //left_low f
    glPushMatrix();
    glColor3f(.5, .5, .5);
    glTranslatef(-100, -55, 2.5);
    glScalef(5, 90, 5);
    glutSolidCube(1);
    glPopMatrix();
}

void gameMap() {
    //borders
    borders();
    //inside border

    //inner_wall_1 g
    glPushMatrix();
    glColor3f(.5, .5, .5);
    glTranslatef(-80, 0, 2.5);
    glScalef(5, 160, 5);
    glutSolidCube(1);
    glPopMatrix();

    //inner_wall_1 h
    glPushMatrix();
    glColor3f(.5, .5, .5);
    glTranslatef(80, 0, 2.5);
    glScalef(5, 160, 5);
    glutSolidCube(1);
    glPopMatrix();

    //inner_upper_wall_right i
    glPushMatrix();
    glColor3f(.5, .5, .5);
    glTranslatef(47.5, 80, 2.5);
    glScalef(70, 5, 5);
    glutSolidCube(1);
    glPopMatrix();

    //inner_upper_wall_left j
    glPushMatrix();
    glColor3f(.5, .5, .5);
    glTranslatef(-47.5, 80, 2.5);
    glScalef(70, 5, 5);
    glutSolidCube(1);
    glPopMatrix();

    //inner_upper_lower_wall_right  k
    glPushMatrix();
    glColor3f(.5, .5, .5);
    glTranslatef(55, 60, 2.5);
    glScalef(50, 5, 5);
    glutSolidCube(1);
    glPopMatrix();

    //inner_upper_lower_wall_left L
    glPushMatrix();
    glColor3f(.5, .5, .5);
    glTranslatef(-37.5, 60, 2.5);
    glScalef(50, 5, 5);
    glutSolidCube(1);
    glPopMatrix();

    //inner_upper_lower_wall_right 2 M
    glPushMatrix();
    glColor3f(.5, .5, .5);
    glTranslatef(37.5, 35, 2.5);
    glScalef(50, 5, 5);
    glutSolidCube(1);
    glPopMatrix();

    //inner_upper_lower_wall_left 2 N
    glPushMatrix();
    glColor3f(.5, .5, .5);
    glTranslatef(-52.5, 35, 2.5);
    glScalef(50, 5, 5);
    glutSolidCube(1);
    glPopMatrix();

    //inner_lower_wall_right O
    glPushMatrix();
    glColor3f(.5, .5, .5);
    glTranslatef(47.5, -80, 2.5);
    glScalef(70, 5, 5);
    glutSolidCube(1);
    glPopMatrix();

    //	inner_lower_wall_left P
    glPushMatrix();
    glColor3f(.5, .5, .5);
    glTranslatef(-47.5, -80, 2.5);
    glScalef(70, 5, 5);
    glutSolidCube(1);
    glPopMatrix();

    //inner_upper_bar left Q
    glPushMatrix();
    glColor3f(.5, .5, .5);
    glTranslatef(-10, 65, 2.5);
    glScalef(5, 65, 5);
    glutSolidCube(1);
    glPopMatrix();

    //inner_upper_bar right R
    glPushMatrix();
    glColor3f(.5, .5, .5);
    glTranslatef(10, 65, 2.5);
    glScalef(5, 65, 5);
    glutSolidCube(1);
    glPopMatrix();

    //inner_lower_bar S
    glPushMatrix();
    glColor3f(.5, .5, .5);
    glTranslatef(0, -57.5, 2.5);
    glScalef(120, 5, 5);
    glutSolidCube(1);
    glPopMatrix();

    //inner_inner_inner_lower_upper T
    glPushMatrix();
    glColor3f(.5, .5, .5);
    glTranslatef(0, -40, 2.5);
    glScalef(120, 5, 5);
    glutSolidCube(1);
    glPopMatrix();

    //inner_inner_inner_lower_upper_upper U
    glPushMatrix();
    glColor3f(.5, .5, .5);
    glTranslatef(0, -30, 2.5);
    glScalef(5, 20, 5);
    glutSolidCube(1);
    glPopMatrix();

    //mid_lower_bar V
    glPushMatrix();
    glColor3f(.5, .5, .5);
    glTranslatef(0, -20, 2.5);
    glScalef(100, 5, 5);
    glutSolidCube(1);
    glPopMatrix();

    //mid_right_bar W
    glPushMatrix();
    glColor3f(.5, .5, .5);
    glTranslatef(50, -2.5, 2.5);
    glScalef(5, 40, 5);
    glutSolidCube(1);
    glPopMatrix();

    //mid_left_bar X
    glPushMatrix();
    glColor3f(.5, .5, .5);
    glTranslatef(-50, -2.5, 2.5);
    glScalef(5, 40, 5);
    glutSolidCube(1);
    glPopMatrix();

    //mid_upper_right_bar Y
    glPushMatrix();
    glColor3f(.5, .5, .5);
    glTranslatef(30, 17.5, 2.5);
    glScalef(45, 5, 5);
    glutSolidCube(1);
    glPopMatrix();
    //mid_upper_left_bar Z
    glPushMatrix();
    glColor3f(.5, .5, .5);
    glTranslatef(-30, 17.5, 2.5);
    glScalef(45, 5, 5);
    glutSolidCube(1);
    glPopMatrix();
}

void pacman() {
    // body
    glPushMatrix();
    glColor3f(1, 1, .4);
    glTranslatef(px, py, 5);
    glScalef(.05, .05, .05);
    glutSolidSphere(100, 100, 100);
    glPopMatrix();

    //changing eye location to show where pacman is going.
    switch (side) {
        case 0: // front
            //eyes left
            glPushMatrix();
            glColor3f(0, 0, 0);
            glTranslatef(px - 1.5, py + 3.5, 7);
            glScalef(1, 1, 1);
            glutSolidDodecahedron();
            glPopMatrix();

            //eyes right
            glPushMatrix();
            glColor3f(0, 0, 0);
            glTranslatef(px + 1.5, py + 3.5, 7);
            glScalef(1, 1, 1);
            glutSolidDodecahedron();
            glPopMatrix();
            break;
        case 1: // back
            //eyes left
            glPushMatrix();
            glColor3f(0, 0, 0);
            glTranslatef(px - 1.5, py - 3.5, 7);
            glScalef(1, 1, 1);
            glutSolidDodecahedron();
            glPopMatrix();

            //eyes right
            glPushMatrix();
            glColor3f(0, 0, 0);
            glTranslatef(px + 1.5, py - 3.5, 7);
            glScalef(1, 1, 1);
            glutSolidDodecahedron();
            glPopMatrix();
            break;
        case 2: //left
            //eyes left
            glPushMatrix();
            glColor3f(0, 0, 0);
            glTranslatef(px + 3.5, py + 1.5, 7);
            glScalef(1, 1, 1);
            glutSolidDodecahedron();
            glPopMatrix();

            //eyes right
            glPushMatrix();
            glColor3f(0, 0, 0);
            glTranslatef(px + 3.5, py - 1.5, 7);
            glScalef(1, 1, 1);
            glutSolidDodecahedron();
            glPopMatrix();
            break;
        case 3: // right
            //eyes left
            glPushMatrix();
            glColor3f(0, 0, 0);
            glTranslatef(px - 3.5, py + 1.5, 7);
            glScalef(1, 1, 1);
            glutSolidDodecahedron();
            glPopMatrix();

            //eyes right
            glPushMatrix();
            glColor3f(0, 0, 0);
            glTranslatef(px - 3.5, py - 1.5, 7);
            glScalef(1, 1, 1);
            glutSolidDodecahedron();
            glPopMatrix();
            break;
        default: //eyes left
            glPushMatrix();
            glColor3f(0, 0, 0);
            glTranslatef(px - 1.5, py + 3.5, 7);
            glScalef(1, 1, 1);
            glutSolidDodecahedron();
            glPopMatrix();

            //eyes right
            glPushMatrix();
            glColor3f(0, 0, 0);
            glTranslatef(px + 1.5, py + 3.5, 7);
            glScalef(1, 1, 1);
            glutSolidDodecahedron();
            glPopMatrix();
            break;
    }
}

int x_coord() {
    return 100 + px;
}

int y_coord() {
    return abs(-100 + py);
}

bool pacmanMove() {
    int temp_x = x_coord();
    int temp_y = y_coord();

    printf("%f\t%f\n", px, py);
    printf("%d\t%d\n", temp_x, temp_y);
    //printf("%d\n", (game_map[temp_x][temp_y] == 'w'));
    //printf("%d\n", (game_map[temp_x][temp_y] == 'w'));
    printf("%d\n", side);
    switch (side) {
        case 0:
            if (!(game_map[temp_y - 5][temp_x] == 'w')) {
                py++;
                game_map[temp_y][temp_x] == 'O';
                game_map[y_coord()][temp_x] == 'p';
            } else if ((game_map[temp_y][temp_x] == 'f')) {
                points++;
                printf("%d", points);
                py++;
                game_map[temp_y][temp_x] == 'O';
                game_map[y_coord()][temp_x] == 'p';
            } else if ((game_map[temp_y][temp_x] == 'i') || (game_map[temp_y][temp_x] == 'b') ||
                       (game_map[temp_y][temp_x] == 'c')) {
                game_map[temp_y][temp_x] == 'X';
                halt_game = true;
            } else {
                side = 4;
                pacmanMove();
            }
            break;
        case 1:
            if (!(game_map[temp_y + 5][temp_x] == 'w')) {
                py--;
                game_map[temp_y][temp_x] == 'O';
                game_map[y_coord()][temp_x] == 'p';
            } else if ((game_map[temp_y][temp_x] == 'f')) {
                points++;
                printf("%d", points);
                py--;
                game_map[temp_y][temp_x] == 'O';
                game_map[y_coord()][temp_x] == 'p';
            } else if ((game_map[temp_y][temp_x] == 'i') || (game_map[temp_y][temp_x] == 'b') ||
                       (game_map[temp_y][temp_x] == 'c')) {
                game_map[temp_y][temp_x] == 'X';
                halt_game = true;
            } else {
                side = 4;
                pacmanMove();
            }
            break;
        case 2:
            if (!(game_map[temp_y][temp_x + 5] == 'w')) {
                px++;
                game_map[temp_y][temp_x] == 'O';
                game_map[temp_y][x_coord()] == 'p';
            } else if ((game_map[temp_y][temp_x] == 'f')) {
                points++;
                printf("%d", points);
                px++;
                game_map[temp_y][temp_x] == 'O';
                game_map[temp_y][x_coord()] == 'p';
            } else if ((game_map[temp_y][temp_x] == 'i') || (game_map[temp_y][temp_x] == 'b') ||
                       (game_map[temp_y][temp_x] == 'c')) {
                game_map[temp_y][temp_x] == 'X';
                halt_game = true;
            } else {
                side = 4;
                pacmanMove();
            }
            break;
        case 3:
            if (!(game_map[temp_y][temp_x - 5] == 'w')) {
                px--;
                game_map[temp_y][temp_x] == 'O';
                game_map[temp_y][x_coord()] == 'p';
            } else if ((game_map[temp_y][temp_x] == 'f')) {
                points++;
                printf("%d", points);
                px--;
                game_map[temp_y][temp_x] == 'O';
                game_map[temp_y][x_coord()] == 'p';
            } else if ((game_map[temp_y][temp_x] == 'i') || (game_map[temp_y][temp_x] == 'b') ||
                       (game_map[temp_y][temp_x] == 'c')) {
                game_map[temp_y][temp_x] == 'X';
                halt_game = true;
            } else {
                side = 4;
                pacmanMove();
            }
            break;

        default:
            break;
    }
    return true;
}

void ghosts(int x, int y, int color) {
    glPushMatrix();
    switch (color) {
        case 1:
            glColor3f(0, .6, 0);
            break;
        case 2:
            glColor3f(1, .5, .4);
            break;
        case 3:
            glColor3f(.6, 0, .6);
            break;
        default:
            glColor3f(0, .2, .8);
            break;
    }
    glTranslatef(x, y, 2.5);
    //	glScalef(.05, .05, .05);
    glScalef(4, 4, 4);
    glutSolidDodecahedron();
    glPopMatrix();
}

void ghostMove_1() {
	//need to update x & y coord methods.
	int temp_x = x_coord();
    int temp_y = y_coord();

    /*printf("%f\t%f\n", px, py);
    printf("%d\t%d\n", temp_x, temp_y);
    printf("%d\n", (game_map[temp_x][temp_y] == 'w'));
    printf("%d\n", (game_map[temp_x][temp_y] == 'w'));
    printf("%d\n", side);*/
    switch (side) {
        case 0:
            if (!(game_map[temp_y - 5][temp_x] == 'w')) {
                py++;
                game_map[temp_y][temp_x] == 'O';
                game_map[y_coord()][temp_x] == 'p';
            } else if ((game_map[temp_y][temp_x] == 'f')) {
                points++;
                printf("%d", points);
                py++;
                game_map[temp_y][temp_x] == 'O';
                game_map[y_coord()][temp_x] == 'p';
            } else if ((game_map[temp_y][temp_x] == 'i') || (game_map[temp_y][temp_x] == 'b') ||
                       (game_map[temp_y][temp_x] == 'c')) {
                game_map[temp_y][temp_x] == 'X';
                halt_game = true;
            } else {
                side = 4;
                pacmanMove();
            }
            break;
        case 1:
            if (!(game_map[temp_y + 5][temp_x] == 'w')) {
                py--;
                game_map[temp_y][temp_x] == 'O';
                game_map[y_coord()][temp_x] == 'p';
            } else if ((game_map[temp_y][temp_x] == 'f')) {
                points++;
                printf("%d", points);
                py--;
                game_map[temp_y][temp_x] == 'O';
                game_map[y_coord()][temp_x] == 'p';
            } else if ((game_map[temp_y][temp_x] == 'i') || (game_map[temp_y][temp_x] == 'b') ||
                       (game_map[temp_y][temp_x] == 'c')) {
                game_map[temp_y][temp_x] == 'X';
                halt_game = true;
            } else {
                side = 4;
                pacmanMove();
            }
            break;
        case 2:
            if (!(game_map[temp_y][temp_x + 5] == 'w')) {
                px++;
                game_map[temp_y][temp_x] == 'O';
                game_map[temp_y][x_coord()] == 'p';
            } else if ((game_map[temp_y][temp_x] == 'f')) {
                points++;
                printf("%d", points);
                px++;
                game_map[temp_y][temp_x] == 'O';
                game_map[temp_y][x_coord()] == 'p';
            } else if ((game_map[temp_y][temp_x] == 'i') || (game_map[temp_y][temp_x] == 'b') ||
                       (game_map[temp_y][temp_x] == 'c')) {
                game_map[temp_y][temp_x] == 'X';
                halt_game = true;
            } else {
                side = 4;
                pacmanMove();
            }
            break;
        case 3:
            if (!(game_map[temp_y][temp_x - 5] == 'w')) {
                px--;
                game_map[temp_y][temp_x] == 'O';
                game_map[temp_y][x_coord()] == 'p';
            } else if ((game_map[temp_y][temp_x] == 'f')) {
                points++;
                printf("%d", points);
                px--;
                game_map[temp_y][temp_x] == 'O';
                game_map[temp_y][x_coord()] == 'p';
            } else if ((game_map[temp_y][temp_x] == 'i') || (game_map[temp_y][temp_x] == 'b') ||
                       (game_map[temp_y][temp_x] == 'c')) {
                game_map[temp_y][temp_x] == 'X';
                halt_game = true;
            } else {
                side = 4;
                pacmanMove();
            }
            break;

        default:
            break;
    }
}

void ghostMove_2() {

}

void ghostMove_3() {

}


void collisionDetect() {

}


void edibles(int x, int y) {
    glPushMatrix();
    glColor3f(0, 1, 0);
    glTranslatef(x, y, 3);
    glScalef(.02, .02, .02);
    glutSolidSphere(100, 100, 100);
    glPopMatrix();
}

bool eat(float x, float y) {

    return false;
}

void generateEdibles() {
    for (int i = 0; i < 200; ++i) {
        for (int j = 0; j < 200; ++j) {
            if (game_map[i][j] == 'f') {
                edibles(100 - j, -100 + i);
            }
        }

    }
}

void init_gamemap_level_one() {
    for (int i = 0; i < mat_range; i++) {
        //cube_A
        for (int j = 0; j <= 4; j++) {
            game_map[j][i] = 'w';
        }
        //cube_B
        for (int j = 195; j <= 199; j++) {
            game_map[j][i] = 'w';
        }
        //cube_D
        if (i <= 4) {
            for (int j = 105; j <= 199; j++) {
                game_map[j][i] = 'w';
            }
        }
        //cube_C
        if (i >= 195) {
            for (int j = 105; j <= 199; j++) {
                game_map[j][i] = 'w';
            }
        }
        //cube_F
        if (i <= 4) {
            for (int j = 0; j <= 95; j++) {
                game_map[j][i] = 'w';
            }
        }
        //cube_E
        if (i >= 195) {
            for (int j = 0; j <= 95; j++) {
                game_map[j][i] = 'w';
            }
        }

        // cube_J
        if (i <= 84 && i >= 14) {
            for (int j = 14; j <= 24; j++) {
                game_map[j][i] = 'w';
            }
        }
        //cube_I
        if (i <= 174 && i >= 104) {
            for (int j = 14; j <= 24; j++) {
                game_map[j][i] = 'w';
            }
        }
        // cube_L
        if (i <= 84 && i >= 34) {
            for (int j = 39; j <= 48; j++) {
                game_map[j][i] = 'w';
            }
        }
        //cube_K
        if (i <= 174 && i >= 134) {
            for (int j = 39; j <= 48; j++) {
                game_map[j][i] = 'w';
            }
        }
        // cube_N
        if (i <= 64 && i >= 14) {
            for (int j = 55; j <= 64; j++) {
                game_map[j][i] = 'w';
            }
        }
        //cube_M
        if (i <= 164 && i >= 114) {
            for (int j = 54; j <= 64; j++) {
                game_map[j][i] = 'w';
            }
        }

        // cube_P
        if (i <= 84 && i >= 14) {
            for (int j = 175; j <= 185; j++) {
                game_map[j][i] = 'w';
            }
        }
        // cube_Q
        if (i <= 94 && i >= 84) {
            for (int j = 0; j <= 64; j++) {
                game_map[j][i] = 'w';
            }
        }
        // cube_R
        if (i <= 114 && i >= 104) {
            for (int j = 0; j <= 64; j++) {
                game_map[j][i] = 'w';
            }
        }

        //cube_O
        if (i <= 174 && i >= 114) {
            for (int j = 175; j <= 185; j++) {
                game_map[j][i] = 'w';
            }
        }

        //cube_S
        if (i <= 164 && i >= 34) {
            for (int j = 155; j <= 165; j++) {
                game_map[j][i] = 'w';
            }
        }

        //cube_T
        if (i <= 164 && i >= 34) {
            for (int j = 135; j <= 145; j++) {
                game_map[j][i] = 'w';
            }
        }

        //cube_U
        if (i <= 104 && i >= 94) {
            for (int j = 125; j <= 135; j++) {
                game_map[j][i] = 'w';
            }
        }

        //cube_v
        if (i <= 149 && i >= 44) {
            for (int j = 115; j <= 125; j++) {
                game_map[j][i] = 'w';
            }
        }

        //cube_X
        if (i <= 49 && i >= 39) {
            for (int j = 75; j <= 115; j++) {
                game_map[j][i] = 'w';
            }
        }

        //cube_W
        if (i <= 164 && i >= 144) {
            for (int j = 75; j <= 115; j++) {
                game_map[j][i] = 'w';
            }
        }

        //cube_G
        if (i <= 24 && i >= 14) {
            for (int j = 14; j <= 179; j++) {
                game_map[j][i] = 'w';
            }
        }
        //cube_H
        if (i <= 184 && i >= 174) {
            for (int j = 14; j <= 179; j++) {
                game_map[j][i] = 'w';
            }
        }

        //cube_Y
        if (i <= 100 && i >= 144) {
            for (int j = 75; j <= 84; j++) {
                game_map[j][i] = 'w';
            }
        }
        //cube_Z
        if (i <= 39 && i >= 74) {
            for (int j = 75; j <= 84; j++) {
                game_map[j][i] = 'w';
            }
        }
    }
}

void display() {
    //codes for Models, Camera

    //clear the display
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 0); //color black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear buffers to preset values

    /***************************
    / set-up camera (view) here
    ****************************/
    //load the correct matrix -- MODEL-VIEW matrix
    glMatrixMode(GL_MODELVIEW); //specify which matrix is the current matrix

    //initialize the matrix
    glLoadIdentity(); //replace the current matrix with the identity matrix [Diagonals have 1, others have 0]

    //now give three info
    //1. where is the camera (viewer)?
    //2. where is the camera looking?
    //3. Which direction is the camera's UP direction?

    //gluLookAt(0,-150,20,	0,0,0,	0,0,1);
    gluLookAt(cameraRadius * sin(cameraAngle), -cameraRadius * cos(cameraAngle), cameraHeight, 0, 0, 0, 0, 0, 1);

    //again select MODEL-VIEW
    glMatrixMode(GL_MODELVIEW);


    /**************************************************
    / Grid and axes Lines(You can remove them if u want)
    ***************************************************/
    //drawGridAndAxes();

    /****************************
    / Add your objects from here
    ****************************/
    init_gamemap_level_one();
    glPushMatrix();
    //glScalef(10,10,10);
    //glScalef(5, 5, 5);
    gameMap();
    pacman();

    ghosts(gx_1, gy_1, 1);
    ghosts(gx_2, gy_2, 2);
    ghosts(gx_3, gy_3, 3);
    glPopMatrix();

    //ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
    glutSwapBuffers();
}


void animate() {
    pacmanMove();
    //printf("Point = %d \n",points);
    generateEdibles();
    //codes for any changes in Models, Camera

    //cameraAngle += cameraAngleDelta;	// camera will rotate at 0.002 radians per frame.

    //codes for any changes in Models
    //printf("%f %f \n",px,py);

    //MISSING SOMETHING? -- YES: add the following
    glutPostRedisplay(); //this will call the display AGAIN
}


void init_gamemap_level_two() {

}

void init() {
    //game level
    game_lvl = 1;
    pacman_life = 3;
    halt_game = false;
    points = 0;
    init_gamemap_level_one();
    //powers
    pw_slow = 0;
    //pacman
    px = 90;
    py = 0;

    //ghosts
    gx_1 = 0;
    gy_1 = 0;

    gx_2 = -20;
    gy_2 = 0;

    gx_3 = 20;
    gy_3 = 0;

    //codes for initialization
    side = 4;
    move_X = 0;
    move_Y = 0;
    move_Z = 0;
    canDrawGrid = 1;
    canDrawAxis = 1;

    cameraAngleDelta = .001;


    cameraAngle = 0; //angle in radian
    cameraRadius = 150;
    cameraHeight = 50;

    //clear the screen
    glClearColor(0, 0, 0, 0);

    /************************
    / set-up projection here
    ************************/
    //load the PROJECTION matrix
    glMatrixMode(GL_PROJECTION);

    //initialize the matrix
    glLoadIdentity();

    /*
        gluPerspective() � set up a perspective projection matrix

        fovy -         Specifies the field of view angle, in degrees, in the y direction.
        aspect ratio - Specifies the aspect ratio that determines the field of view in the x direction. The aspect ratio is the ratio of x (width) to y (height).
        zNear -        Specifies the distance from the viewer to the near clipping plane (always positive).
        zFar  -        Specifies the distance from the viewer to the far clipping plane (always positive).
    */

    gluPerspective(70, 1, 0.1, 10000.0);
}

void keyboardListener(unsigned char key, int x, int y) {
    float tempX = px;
    float tempY = py;

    switch (key) {
        case '1':
            move_X += 1;
            break;

        case '2':
            move_X -= 1;
            break;

        case '3':
            move_Y += 1;
            break;
        case '4':
            move_Y -= 1;
            break;

        case '5':
            move_Z += 1;
            break;
        case '6':
            move_Z -= 1;
            break;
        case '8':

            break;

        case 'g':
            canDrawGrid ^= 1;
            break;

        case 'h':
            canDrawAxis ^= 1;
            break;

        case 'i':
            side = 0;
            break;

        case 'k':
            side = 1;
            break;

        case 'j':
            side = 3;
            break;

        case 'l':
            side = 2;
            break;

        default:
            break;
    }
}

void specialKeyListener(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_DOWN: //down arrow key
            cameraRadius += 10;
            break;
        case GLUT_KEY_UP: // up arrow key
            if (cameraRadius > 10)
                cameraRadius -= 10;
            break;

        case GLUT_KEY_RIGHT:
            cameraAngle += 0.01;
            break;
        case GLUT_KEY_LEFT:
            cameraAngle -= 0.01;
            break;

        case GLUT_KEY_PAGE_UP:
            cameraHeight += 10;
            break;
        case GLUT_KEY_PAGE_DOWN:
            cameraHeight -= 10;
            break;

        case GLUT_KEY_INSERT:
            break;

        case GLUT_KEY_HOME:
            cameraAngle = 0;
            cameraRadius = 150;
            cameraHeight = 50;
            break;
        case GLUT_KEY_END:

            break;

        default:
            break;
    }
}

void mouseListener(int button, int state, int x, int y) { //x, y is the x-y of the screen (2D)
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN) { // 2 times?? in ONE click? -- solution is checking DOWN or UP
                cameraAngleDelta = -cameraAngleDelta;
            }
            break;

        case GLUT_RIGHT_BUTTON:
            //........
            break;

        case GLUT_MIDDLE_BUTTON:
            //........
            break;

        default:
            break;
    }
}

int main(int argc, char **argv) {
    glutInit(&argc, argv); //initialize the GLUT library

    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);

    /*
        glutInitDisplayMode - inits display mode
        GLUT_DOUBLE - allows for display on the double buffer window
        GLUT_RGBA - shows color (Red, green, blue) and an alpha
        GLUT_DEPTH - allows for depth buffer
    */
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);

    glutCreateWindow("Some Title");

    printf("Camera Control\n");
    printf("_____________\n");
    printf("Zoom In-Out: UP and DOWN arrow\n");
    printf("Camera Rotate: LEFT and RIGHT arrow\n");
    printf("Up-Down: PAGEUP and PAGEDOWN\n");
    printf("Reset Camera: HOME\n");

    init(); //codes for initialization

    glEnable(GL_DEPTH_TEST); //enable Depth Testing

    glutDisplayFunc(display); //display callback function
    glutIdleFunc(animate); //what you want to do in the idle time (when no drawing is occuring)

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);

    glutMouseFunc(mouseListener);

    glutMainLoop(); //The main loop of OpenGL

    return 0;
}

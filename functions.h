/* ============== 2/2 fucntions.h ======================|
>> Functions header needed for 68848_66305_project.cpp  |
    program 		                                    |
>> Metric #s:	68848 Mahmoud Kassab				    |
>> Include with main file inside Visual studio  	    |
// =================================================== */

#pragma once
#pragma warning(disable : 4996) // to ignore fileopen_s error

// refresh interval in milliseconds 1000 = 1 sec
int refreshMills = 200; 
//starting positions of the objects, from 0 to 360 degrees.
static int shoulder = 0, elbow = 0, elbowY = 0;
bool doneMov = false, rotateCan = false, closeHands = false;
// camera positions
GLfloat cameraX = 1.0f, cameraY = 1.3f, cameraZ = 2.0f; // x=2.3f
// can position
GLfloat canX = 0.85, canY = 0.6, canZ = 0.7;
// hands position
GLfloat handXT = -0.3f, handXL = -0.7f;

#define PI 3.1415927
GLuint texture; //the array for our texture
GLfloat angle = 0.0;

//function to load the RAW file
GLuint LoadTexture(const char* filename, int width, int height)
{
    GLuint texture;
    unsigned char* data;
    FILE* file;

    //The following code will read in our RAW file
    file = fopen(filename, "rb");
    if (file == NULL) return 0;
    data = (unsigned char*)malloc(width * height * 3);
    fread(data, width * height * 3, 1, file);
    fclose(file);

    glGenTextures(1, &texture); //generate the texture with  the loaded data
    glBindTexture(GL_TEXTURE_2D, texture); //bind the texture to it�s array
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,
        GL_MODULATE); //set texture environment parameters

            //here we are setting what textures to use and when. The MIN  filter is which quality to show
    //when the texture is near the view, and the MAG filter is which  quality to show when the texture
    //is far from the view.

    //The qualities are (in order from worst to best)
    //GL_NEAREST
    //GL_LINEAR
    //GL_LINEAR_MIPMAP_NEAREST
    //GL_LINEAR_MIPMAP_LINEAR

    //And if you go and use extensions, you can use Anisotropic  filtering textures which are of an
    //even better quality, but this will do for now.
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //Here we are setting the parameter to repeat the texture  instead of clamping the texture
        //to the edge of our shape. 
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT);

    //Generate the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    free(data); //free the texture
    return texture; //return whether it was successfull
}

void FreeTexture(GLuint texture) // delete texure after finish
{
    glDeleteTextures(1, &texture);
}

void draw_cylinder(GLfloat radius, GLfloat height, GLubyte R, GLubyte G, GLubyte B)
{
    //Load our texture
    texture = LoadTexture("texture.bmp", 256, 256); // texture file and size

    glEnable(GL_TEXTURE_2D);    //enable texture for the can
    glBindTexture(GL_TEXTURE_2D, texture); //bind our texture to our shape

    GLfloat x = 0.0;
    GLfloat y = 0.0;
    GLfloat angle = 0.0;
    GLfloat angle_stepsize = 0.1;

    /** Draw the tube */
    glColor3ub(R - 40, G - 40, B - 40);
    glBegin(GL_QUAD_STRIP);
        angle = 0.0;
        while (angle < 2 * PI) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glTexCoord2d(0.0, 0.0);
            glVertex3f(x, y, height);
            glTexCoord2d(1.0, 0.0);
            glVertex3f(x, y, 0.0);
            angle = angle + angle_stepsize;
        }
        glTexCoord2d(1.0, 1.0);
        glVertex3f(radius, 0.0, height);
        glTexCoord2d(0.0, 1.0);
        glVertex3f(radius, 0.0, 0.0);
    glEnd();

    /** Draw the circle on top of cylinder */
    glColor3ub(R, G, B);
        glBegin(GL_POLYGON);
        angle = 0.0;
        while (angle < 2 * PI) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glVertex3f(x, y, height);
            angle = angle + angle_stepsize;
        }

        glVertex3f(radius, 0.0, height);
    glEnd();
}

void DrawUnitCylinder(int numSegs)  // x,y,z in [0,1], Y-axis is up
{
    int i;
    GLfloat Px[16];
    GLfloat Py[16];
    GLfloat AngIncr = (2.0f * 3.1415927f) / (GLfloat)numSegs;
    GLfloat Ang = AngIncr;
    Px[0] = 1;

    Py[0] = 0;

    for (i = 1; i < numSegs; i++, Ang += AngIncr)
    {
        Px[i] = (GLfloat)cos(Ang);
        Py[i] = (GLfloat)sin(Ang);
    }

    glMatrixMode(GL_MODELVIEW); // Applies subsequent matrix operations to the modelview matrix stack. 
    glPushMatrix();
        glTranslatef(0.5f, 0.5f, 0.5f);
        glScalef(0.5f, 0.5f, 0.5f);

        // Top
        glNormal3f(0, 1, 0);
        glBegin(GL_TRIANGLE_FAN);
            glVertex3f(0, 1, 0);
            for (i = 0; i < numSegs; i++)
                glVertex3f(Px[i], 1, -Py[i]);
            glVertex3f(Px[0], 1, -Py[0]);
        glEnd();

        // Bottom
        glNormal3f(0, -1, 0);   //The current normal is set to the given coordinates
        glBegin(GL_TRIANGLE_FAN);
            glVertex3f(0, -1, 0);
            for (i = 0; i < numSegs; i++)
                glVertex3f(Px[i], -1, Py[i]);
            glVertex3f(Px[0], -1, Py[0]);
        glEnd();

        // Sides
        glBegin(GL_QUAD_STRIP);
            for (i = 0; i < numSegs; i++)
            {
                glNormal3f(Px[i], 0, -Py[i]);
                glVertex3f(Px[i], 1, -Py[i]);
                glVertex3f(Px[i], -1, -Py[i]);
            }
            glNormal3f(Px[0], 0, -Py[0]);
            glVertex3f(Px[0], 1, -Py[0]);
            glVertex3f(Px[0], -1, -Py[0]);
        glEnd();
    glPopMatrix();
}



/* Called back when timer expired */
void Timer(int value) {
    glutPostRedisplay();      // Post re-paint request to activate display()
    glutTimerFunc(refreshMills, Timer, 0); // next Timer call milliseconds later
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -5.0);
}

void animation() {
    // ------------------------------------------ Robot Hand, Can Movement ---------------------
    if (cameraX < 2.5f) { // moving camera to position
        cameraX += 0.1;
    }
    else { // start robot movement after camera in position
        if (!doneMov) { // hand movement to move can
            if (elbow < 180) { // move hand down
                elbow += 10;
            }
            else { // if rached down, change rotation to horizontally
                elbowY = 1;
            }
            if (elbowY && elbow < 270) { // rotating horizontally
                // close hands on can
                if (!closeHands) {
                    handXT -= 0.1;
                    handXL += 0.1;
                    closeHands = true;
                }

                elbow += 10;

                if (canZ <= 1.0) { // move can with hands
                    canZ += 0.05;
                    if (canZ > 0.9) { // rotate can with hands
                        rotateCan = true;
                        canX += 0.05;
                    }
                }
                else if (canZ > 1.0 && canX < 1.1) { // can movement on x
                    canX += 0.05;
                }
            }
            else if (elbow == 270) { // when finish set doneMov
                doneMov = true;
            }
        }
        else { // hand movement to go back to first position
            if (elbow > 180 && elbowY) { // keep rotating horizontally 
                elbow -= 10;
            }
            else if (elbow == 180) { // if reached 180, rotate vertically to go up
                elbowY = 0;
            }
            if (elbow > 0 && elbowY == 0) { // rotate up to first position
                elbow -= 10;
                if (closeHands) { // open robot hands
                    handXT += 0.1;
                    handXL -= 0.1;
                    closeHands = false;
                }
            }
        } // end movedown else
    }// end camera else
}


void wall(double thickness) // to draw walls and floor
{
    glPushMatrix();
        glTranslated(0.5, 0.5 * thickness, 0.5);
        glScaled(1.0, thickness, 1.0);
        glutSolidCube(1.0);
    glPopMatrix();
}

void tableLeg(double thick, double len) // will be used to draw the table legs
{
    glPushMatrix();
        glTranslated(0, len / 2, 0);
        glScaled(thick, len, thick);
        glutSolidCube(1.0);
    glPopMatrix();
}

void table(double topWid, double topThick, double legThick, double legLen)
{
    // table top
    glPushMatrix();
        glTranslated(0, legLen, 0);
        glScaled(topWid, topThick, topWid);
        glutSolidCube(1.0);
    glPopMatrix();

    double dist = 0.95 * topWid / 2.0 - legThick / 2.0;

    // table four legs
    glPushMatrix();
        glTranslated(dist, 0, dist);
        tableLeg(legThick, legLen);
        glTranslated(0.0, 0.0, -2 * dist);
        tableLeg(legThick, legLen);
        glTranslated(-2 * dist, 0, 2 * dist);
        tableLeg(legThick, legLen);
        glTranslated(0, 0, -2 * dist);
        tableLeg(legThick, legLen);
    glPopMatrix();
}

void DrawUnitSphere(int numSegs)  // x,y,z in [0,1]
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        glTranslatef(0.6f, 0.5f, 0.5f);
        glutSolidSphere(0.5f, numSegs, numSegs);
    glPopMatrix();
}

void DrawFingerBase(int numSegs)    // used to draw the hand sticks
{
    // draw the stick
    glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glScalef(0.1f, 1.5f, 0.1f);
        DrawUnitCylinder(numSegs);
    glPopMatrix();

    // draw stick base
    glPushMatrix();
        glTranslatef(0, -0.1f, 0);
        glScalef(0.2f, 0.5f, 0.5f);
        glTranslatef(-0.5f, -0.5f, -0.5f);
        DrawUnitSphere(numSegs);
    glPopMatrix();
}

void keyboard(unsigned char key, int x, int y)  // keyboard function to controll manually, need to disable animation() in display
{
    switch (key) {
    case 'q': // change hand movement to horizontal
        elbowY = 1;
        glutPostRedisplay();
        break;
    case 'a':   // change hand movement to vertical
        elbowY = 0;
        glutPostRedisplay();
        break;
    case 'e':   // move camera +ve x
        cameraX += 0.1; //0.05
        glutPostRedisplay();
        break;
    case 'd':   // move camera -ve x
        cameraX -= 0.1;
        glutPostRedisplay();
        break;
    case 'w':   // move hand down or rotate left
        elbow += 10;
        glutPostRedisplay();
        break;
    case 's':   // move hand up or rotate right
        elbow -= 10;
        glutPostRedisplay();
        break;
    case 'r':   // close top hand stick
        handXT -= 0.05;
        glutPostRedisplay();
        break;
    case 'f':   // close lower hand stick
        handXL += 0.05;
        glutPostRedisplay();
        break;
    case 27:    // close when click escape on keyboard
        exit(0);
        break;
    default:
        break;
    }
}

/* ============== 1/2 68848_66305_project.cpp ==========|
>> OpenGL Final Project: Robot hand, can, table		    |
>> Metric #s:	68848 Mahmoud Kassab				    |
>> Description: robot hand to move a can on a table	    |
// =================================================== */
#include "initials.h"   // loading initials header
#include "functions.h" // must include with this file in visual studio, has all functions needed


void display(void)
{
    GLfloat mat_ambient[] = { 0.5f, 0.5f, 1.7f, 1.0f };
    GLfloat mat_diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat mat_shininess[] = { 10.0f };

    //The glMaterialfv function specifies material parameters for the lighting model.
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);    
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    GLfloat lightIntensity[] = { 0.7f, 0.7f, 0.7f, 0.7f };
    GLfloat light_position[] = { 2.0f, 6.0f, 3.0f, 0.0f };

    //The glLightfv function returns light source parameter values.
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);

    glMatrixMode(GL_PROJECTION); // switches to the projection matrix 
    glLoadIdentity();   //Cleans out the current matrix, replacing it with the identity
    double winHt = 1.0;
    glOrtho(-winHt * 64 / 48.0, winHt * 64 / 48.0, -winHt, winHt, 0.1, 100.0);  // transformation that produces a parallel projection
    glMatrixMode(GL_MODELVIEW); // contain modeling and viewing transformations
    glLoadIdentity();
    gluLookAt( cameraX, cameraY, cameraZ, /* look from camera XYZ */
               0.0, 0.25, 0.0, /* look at the origin */ 
               0.0, 1.0, 0.0   /* positive Y up vector */
             ); //viewing transformation
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear color and depth bufers

    //base -> Red
    glPushMatrix(); /* save old coordinate system */
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient - 3); // change ambient color to base
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse - 4); // change difuse to base
        glTranslated(1.2, 0.6, 0.8);
        glScalef(0.2, 0.3, 0.2);
        glutSolidCube(1.0);
    glPopMatrix();  /* restore old coordinate system */

    //---------------------------------------------  Robot Arm ------------------------------------
    glPushMatrix(); 

        if (elbowY) {   // rotate the arm horizontally
            glTranslatef(1.2, 0.74, 0.85);
            glRotatef((GLfloat)elbow, 0.0, 1.0, 0.0);
            glScalef(0.5, 0.1, 0.2);
            glTranslatef(0.2, 0.1, 0.2);
            glTranslatef(0.0, 0.0, 0.0);
        }
        else {  // rotate the arm vertically
            glTranslatef(1.2, 0.75, 0.75);
            glRotatef((GLfloat)elbow, 0.0, 0.0, 1.0);
            glScalef(0.5, 0.1, 0.2);
            glTranslatef(0.2, 0.1, 0.2);
            glTranslatef(0.0, 0.0, 0.0);
        }

   
        //shoulder -> Green
        glPushMatrix();
            glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient - 1); // change ambient color to shoulder
            glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse - 4); // change difuse to shoulder
            glutSolidCube(0.7);
        glPopMatrix();

        //Hand -> Yellow
        glPushMatrix();
            glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient + 1); // change ambient color to hand
            glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse - 4); // change difuse to hand
            glTranslated(0.5, 0.0, 0.0);
            glutSolidCube(0.75);
        glPopMatrix();

        // ---------------------------------------- Sticks ---------------------------------------
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient + 2); // change ambient color to hand
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse - 3); // change difuse to hand
    
        glTranslatef(0.95, 0.0, 0.0);

        // top hand stick  
        glPushMatrix();
            glTranslatef(handXT, 0.45f, 0.0f);  // translate the top hand stick with the arm movement
            if (elbowY) { // adjust the top hand stick to the arm rotation and translation
                glRotatef(180, 1.0, 0.0, 0.0);
                glTranslatef(0.0, 0.9f, 0.0f);
            }
            DrawFingerBase(16);
        glPopMatrix();

        // lower hand stick
        glPushMatrix();
            glTranslatef(handXL, 0.45f, 0.0f);  // translate the top hand stick with the arm movement
            if (elbowY) {   // adjust the top hand stick to the arm rotation and translation
                glRotatef(180, 1.0, 0.0, 0.0);
                glTranslatef(0.0, 0.9f, 0.0f);
            }
            DrawFingerBase(16);
        glPopMatrix();
    
    glPopMatrix();

    // ---------------------------------------------- Can ---------------------------------------
    glPushMatrix();
    if (rotateCan) {    //rotate the can when the arm move to the another end of the table
        glTranslated(canX, canY, canZ+0.15);
        glRotatef(90, 0.0, 1.0, 0.0); 
        glTranslated(0.0, 0.0, 0.0);
    }
    else {  // normal position of the can
        glTranslated(canX, canY, canZ);

    }
    draw_cylinder(0.05, 0.2, 255, 160, 100);    // drawa the can as cylinder with some texture
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);   // disable texture after applying to can, so walls and table not affected.

    // --------------------------------------- Table ------------------------------------------
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient); // change ambient color to table
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse); // change difuse to table
    
    glPushMatrix();
        glTranslated(0.25, 0.42, 0.35);
    glPopMatrix();
    glPushMatrix();
        glTranslated(1.0, 0, 1.0);
        table(1, 0.05, 0.05, 0.5);
    glPopMatrix();

    //--------------------------------- Walls ------------------------------------------------
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient - 1); // change ambient color to wall
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse - 5); // change difuse to wall
    
    glScalef(3, 3, 3); // scale the wall x2
    wall(0.01); // draw floor

    glPushMatrix();
        glRotated(90.0, 0.0, 0.0, 1.0);    //draw left wall
        glScalef(2, 2, 2); // scale the wall x2
        wall(0.01);
    glPopMatrix();

    glPushMatrix();
        glRotated(-90.0, 1.0, 0.0, 0.0);   //draw right wall
        glScalef(2, 2, 2); // scale the wall x2
        wall(0.01);
    glPopMatrix();
    //------------------------------------ End ------------------------------------------
    glFlush();

    animation();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1024, 680);
    glutInitWindowPosition(100, 0);
    glutCreateWindow("68848_66305_project");
    glutDisplayFunc(display);
    
    glEnable(GL_LIGHTING);  //associate the current color or index with each vertex
    glEnable(GL_LIGHT0);    // include light 0 in the evaluation of the lighting equation
    glShadeModel(GL_SMOOTH);    // this shading causes the computed colors of vertices to be interpolated as the primitive is rasterized
    glEnable(GL_DEPTH_TEST);    //do depth comparisons and update the depth buffer.
    glEnable(GL_NORMALIZE);     // normal vectors are normalized to unit length after transformation and before lighting.
    
    glClearColor(0.1, 0.1, 0.1, 0.0);   //specifies the red, green, blue, and alpha values used by glClear to clear the color buffers.
    glViewport(0, 0, 640, 480);     // specifies the affine transformation of x and y from normalized device coordinates to window coordinates.

    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, Timer, 0);

    glutMainLoop();
}
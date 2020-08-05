#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static GLfloat rotate = 0.0;

void init(void)
{
    // set background color to black
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    // Rotate around y axis
    glRotatef((GLfloat)rotate, 0.0, 1.0, 0.0);

    // glBindTexture(GL_TEXTURE_2D, 1);
    glEnable(GL_TEXTURE_2D);
    GLUquadric *quad = gluNewQuadric();
    gluQuadricTexture(quad, 1);
    gluSphere(quad, 5, 20, 20);
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void do_lights()
{
    float light0_ambient[] = {0.0, 0.0, 1.0, 1.0};
    float light0_diffuse[] = {0.0, 0.0, 1.0, 10.0};
    float light0_specular[] = {1.0, 1.0, 10.0, 10.0};
    float light0_position[] = {0.0, 0.0, 1.0, 0.0};
    float light0_direction[] = {1.0, 1.0, 1.0, 1.0};

    // Turn off scene default ambient.
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light0_ambient);

    // Make specular correct for spots.
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);

    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 150);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 120.0);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light0_direction);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void do_material()
{
    float mat_ambient[] = {0.0, 0.0, 1.1, 10.0};
    float mat_diffuse[] = {0.8, 0.8, 0.8, 5.0};
    float mat_specular[] = {0.5, 0.5, 0.5, 20.0};
    float mat_shininess[] = {3.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
}

void load_texture()
{
    FILE *fopen(), *fptr;
    char buf[512];
    int im_size, im_width, im_height, max_color;
    unsigned char *texture_bytes;
    char *parse;

    // Load a ppm file and hand it off to the graphics card.
    fptr = fopen("scuff.ppm", "r");
    fgets(buf, 512, fptr);
    do
    {
        fgets(buf, 512, fptr);
    } while (buf[0] == '#');
    parse = strtok(buf, " ");
    im_width = atoi(parse);

    parse = strtok(NULL, "\n");
    im_height = atoi(parse);

    fgets(buf, 512, fptr);
    parse = strtok(buf, " ");
    max_color = atoi(parse);

    im_size = im_width * im_height;
    texture_bytes = (unsigned char *)calloc(3, im_size);
    fread(texture_bytes, 1, 3 * im_size, fptr);
    fclose(fptr);

    glBindTexture(GL_TEXTURE_2D, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, im_width, im_height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, texture_bytes);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    // We don't need this copy; the graphics card has its own now.
    cfree(texture_bytes);
}

void cleanup()
{
    // Release resources here.
    exit(0);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    // esc(27) or press q key to quit program
    case 27:
    case 'q':
        cleanup();
    default:
        break;
    }
}

void update(int value)
{
    rotate += 2.0f;
    if (rotate > 360.f)
    {
        rotate -= 360;
    }
    glutPostRedisplay();
    glutTimerFunc(25, update, 0);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    // glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    // glutInitWindowPosition(200, 50);
    // put display to the middle of window
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - 500) / 2,
                           (glutGet(GLUT_SCREEN_HEIGHT) - 500) / 2);
    glutCreateWindow(argv[0]);
    init();
    load_texture();
    do_lights();
    do_material();
    glutDisplayFunc(display);
    glutTimerFunc(25, update, 0);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}

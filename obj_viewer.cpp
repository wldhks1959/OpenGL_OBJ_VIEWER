#include <vector>
#include <string>
#include <sstream> // istringstream iss(s);
#include <fstream>
#include <iostream>
#include <glut.h>
#include <gl/gl.h>
#include <gl/glu.h>

using namespace std;

GLint xValue = 0;
GLint yValue = 0;
GLint zValue = 0;

GLint clickDown = 0;
GLint fixX = 0;
GLint fixY = 0;

class obj3dmodel
{
    //����
    struct vertex {
        double x;
        double y;
        double z;
    };
    //���� ����
    struct normal {
        double nx;
        double ny;
        double nz;
    };
    //��
    struct face {
        unsigned int v1, v2, v3;
        unsigned int n1, n2, n3;
    };
    vector<vertex> vertices;
    vector<normal> normals;
    vector<face> faces;

public:
    void readfile(const char* filename); //���ϸ��� �޾�, ����ü�� �°� �Ľ��Ͽ� ��� ������ ���� 
    void draw(); // ����� �����͸� �̿��Ͽ� 3D ���� ������ 
};

obj3dmodel model;

void obj3dmodel::readfile(const char* filename)
{
    string s;
    ifstream fin(filename);    // ������ �б� ���� ifstream ���

    if (!fin)
    {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    //OBJ�κ��� �����͸� �а� �ִ��� Ȯ���ϱ� ���� ����
    int vertexCount = 0;
    int normalCount = 0;
    int faceCount = 0;

    while (getline(fin, s))
    {
        if (s.empty() || s[0] == '#') //s�� ����ְų�, �ּ�(#�� �����ϴ� ���ڿ�)�̸� ����
            continue;

        istringstream iss(s); // ���ڿ��� �б� ����, iss ��ü ���� (�Ľ��� ����)
        string type;
        iss >> type;

        if (type == "v")    // ���� ����
        {
            vertex v;
            iss >> v.x >> v.y >> v.z;
            this->vertices.push_back(v);
            vertexCount++;
        }
        else if (type == "vn")   // ���� ���� ����
        {
            normal n;
            iss >> n.nx >> n.ny >> n.nz;
            this->normals.push_back(n);
            normalCount++;
        }
        else if (type == "f")    // �� ����
        {
            face f;
            char dummy; // ������� ���� ���ڸ� ���� 
            iss >> f.v1 >> dummy >> dummy >> f.n1 >> f.v2 >> dummy >> dummy >> f.n2 >> f.v3 >> dummy >> dummy >> f.n3;
            f.v1--; // 1�� ���� 0���� �����ϵ��� ��ȯ
            f.v2--;
            f.v3--;
            f.n1--;
            f.n2--;
            f.n3--;
            faces.push_back(f);
            faceCount++;
        }
    }
    //OBJ ���Ϸκ��� ���������� �����͸� �о�´ٸ� �� �����͸� ���
    cout << "Vertex count: " << vertexCount << endl;
    cout << "Normal count: " << normalCount << endl;
    cout << "Face count: " << faceCount << endl;
}

void obj3dmodel::draw()
{
    glLineWidth(0.01f);
    glBegin(GL_LINES);

    for (int i = 0; i < faces.size(); i++)
    {
        vertex v1 = vertices[faces[i].v1];
        vertex v2 = vertices[faces[i].v2];
        vertex v3 = vertices[faces[i].v3];

        // Draw the lines of each face
        glVertex3f(v1.x, v1.y, v1.z);
        glVertex3f(v2.x, v2.y, v2.z);

        glVertex3f(v2.x, v2.y, v2.z);
        glVertex3f(v3.x, v3.y, v3.z);

        glVertex3f(v3.x, v3.y, v3.z);
        glVertex3f(v1.x, v1.y, v1.z);
    }
    glEnd();
}

void MyDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // ��ġ�� ���� ��ǥ�൵ ȸ��
    glRotatef(xValue, 1.0, 0.0, 0.0);
    glRotatef(yValue, 0.0, 1.0, 0.0);
    glRotatef(zValue, 0.0, 0.0, 1.0);

    // �þ� ����
    glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);

    gluLookAt(0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glColor3f(1.0, 1.0, 1.0); // ���� ���� (���)

    glScalef(2.0f, 2.0f, 2.0f);

    // obj3dmodel Ŭ������ draw �Լ� ȣ���Ͽ� �� ������
    model.draw();

    glFlush();
}

//���콺 �� Ű���� �̺�Ʈ 
void MyKeyboard(unsigned char KeyPressed, int X, int Y)
{
    switch (KeyPressed)
    {
    case 'X':
    case 'x':
        xValue += 1;
        if (xValue > 360) xValue -= 360;
        break;
    case 'Y':
    case 'y':
        yValue += 1;
        if (yValue > 360) yValue -= 360;
        break;
    case 'Z':
    case 'z':
        zValue += 1;
        if (zValue > 360) zValue -= 360;
        break;
    case 'Q':
    case 'q':
        exit(0);
        break;
    case 32: // SPACE Key
        xValue = 0;
        yValue = 0;
        zValue = 0;
        break;
    case 27: // ESC Key
        exit(0);
        break;
    case 3: // CTRL + C
        exit(0);
        break;
    }
    printf("%d\n", KeyPressed);
    glutPostRedisplay();
}

void MyMouseClick(GLint Button, GLint State, GLint X, GLint Y)
{
    if (Button == GLUT_LEFT_BUTTON && State == GLUT_DOWN)
    {
        fixX = X;
        fixY = Y;
        clickDown = 1;
    }
    if (Button == GLUT_LEFT_BUTTON && State == GLUT_UP)
    {
        clickDown = 0;
    }
}

void MyMouseMove(GLint X, GLint Y)
{
    if (clickDown == 1)
    {
        xValue += (fixX - X);
        yValue += (fixY - Y);
        fixX = X;
        fixY = Y;
    }
    glutPostRedisplay();
}

int main(int argc, char* argv[])
{
    // ����� obj ���� ���
    //bear_bis.obj , kitten.obj , homer.obj
    const char* filename = "C:\\Users\\�׷��Ƚ�������\\OneDrive - ���ƴ��б�\\���� ȭ��\\4���� ����\\new_obj\\bear_bis.obj";

    // obj ���Ͽ��� ������ �о����
    model.readfile(filename);

    // GLUT �ʱ�ȭ
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(2000, 1600);
    glutCreateWindow("OBJ Model Viewer");

    glutDisplayFunc(MyDisplay);
    glutKeyboardFunc(MyKeyboard);
    glutMouseFunc(MyMouseClick);
    glutMotionFunc(MyMouseMove);

    // ���� �׽�Ʈ Ȱ��ȭ
    glEnable(GL_DEPTH_TEST);

    glutMainLoop();
    return 0;
}
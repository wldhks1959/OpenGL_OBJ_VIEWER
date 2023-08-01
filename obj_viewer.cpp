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
    //정점
    struct vertex {
        double x;
        double y;
        double z;
    };
    //법선 벡터
    struct normal {
        double nx;
        double ny;
        double nz;
    };
    //면
    struct face {
        unsigned int v1, v2, v3;
        unsigned int n1, n2, n3;
    };
    vector<vertex> vertices;
    vector<normal> normals;
    vector<face> faces;

public:
    void readfile(const char* filename); //파일명을 받아, 구조체에 맞게 파싱하여 멤버 변수에 저장 
    void draw(); // 저장된 데이터를 이용하여 3D 모델을 렌더링 
};

obj3dmodel model;

void obj3dmodel::readfile(const char* filename)
{
    string s;
    ifstream fin(filename);    // 파일을 읽기 위해 ifstream 사용

    if (!fin)
    {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    //OBJ로부터 데이터를 읽고 있는지 확인하기 위한 변수
    int vertexCount = 0;
    int normalCount = 0;
    int faceCount = 0;

    while (getline(fin, s))
    {
        if (s.empty() || s[0] == '#') //s가 비어있거나, 주석(#로 시작하는 문자열)이면 무시
            continue;

        istringstream iss(s); // 문자열을 읽기 위해, iss 객체 생성 (파싱을 위해)
        string type;
        iss >> type;

        if (type == "v")    // 정점 정보
        {
            vertex v;
            iss >> v.x >> v.y >> v.z;
            this->vertices.push_back(v);
            vertexCount++;
        }
        else if (type == "vn")   // 법선 벡터 정보
        {
            normal n;
            iss >> n.nx >> n.ny >> n.nz;
            this->normals.push_back(n);
            normalCount++;
        }
        else if (type == "f")    // 면 정보
        {
            face f;
            char dummy; // 사용하지 않을 문자를 저장 
            iss >> f.v1 >> dummy >> dummy >> f.n1 >> f.v2 >> dummy >> dummy >> f.n2 >> f.v3 >> dummy >> dummy >> f.n3;
            f.v1--; // 1을 빼서 0부터 시작하도록 변환
            f.v2--;
            f.v3--;
            f.n1--;
            f.n2--;
            f.n3--;
            faces.push_back(f);
            faceCount++;
        }
    }
    //OBJ 파일로부터 정상적으로 데이터를 읽어온다면 그 데이터를 출력
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

    // 위치에 따라서 좌표축도 회전
    glRotatef(xValue, 1.0, 0.0, 0.0);
    glRotatef(yValue, 0.0, 1.0, 0.0);
    glRotatef(zValue, 0.0, 0.0, 1.0);

    // 시야 설정
    glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);

    gluLookAt(0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glColor3f(1.0, 1.0, 1.0); // 모델의 색상 (흰색)

    glScalef(2.0f, 2.0f, 2.0f);

    // obj3dmodel 클래스의 draw 함수 호출하여 모델 렌더링
    model.draw();

    glFlush();
}

//마우스 및 키보드 이벤트 
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
        xValue += (fixY - Y);
        yValue += (fixX - X);
        fixX = X;
        fixY = Y;
    }
    glutPostRedisplay();
}

int main(int argc, char* argv[])
{
    // 사용할 obj 파일 경로
    //bear_bis.obj , kitten.obj , homer.obj
    const char* filename = "Your file path\\model.obj";

    // obj 파일에서 데이터 읽어오기
    model.readfile(filename);

    // GLUT 초기화
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(2000, 1600);
    glutCreateWindow("OBJ Model Viewer");

    glutDisplayFunc(MyDisplay);
    glutKeyboardFunc(MyKeyboard);
    glutMouseFunc(MyMouseClick);
    glutMotionFunc(MyMouseMove);

    // 깊이 테스트 활성화
    glEnable(GL_DEPTH_TEST);

    glutMainLoop();
    return 0;
}

// **********************************************************************
// PUCRS/FACIN
// COMPUTA��O GR�FICA
//
// Teste de colis�o em OpenGL
//
// Marcio Sarroglia Pinho
// pinho@inf.pucrs.br
// **********************************************************************
#include <iostream>
#include <iomanip>
#include <cmath>
#include <ctime>

using namespace std;

#ifdef WIN32
#include <windows.h>
#include <glut.h>
#else
#include <sys/time.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <glut.h>
#endif

#include "Ponto.h"
#include "Linha.h"

const int MAX = 300;
bool devoTestar = true;
bool devoExibir = true;
bool devoImprimirFPS = false;

Linha Linhas[MAX];
Linha Veiculo;

float tx, ty, alfa;
void PrintMenu()
{
    cout << "f - imprime FPS." << endl;
    cout << "ESPACO - liga/desliga teste de colisao." << endl;
}
// ***********************************************
// imprime a matriz de transforma�ao da OpenGL
// ***********************************************
void imprimeMatrizGL()
{

    GLfloat matriz_gl[4][4];

    glGetFloatv(GL_MODELVIEW_MATRIX,&matriz_gl[0][0]);

    for(int i = 0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            cout << setw(5) << matriz_gl[i][j] << " ";
        }
        cout << endl;
    }
}
// ***********************************************
//  void InstanciaPonto(Ponto p, Ponto &out)
//
//  Esta fun��o calcula as coordenadas
//  de um ponto no sistema de refer�ncia do
//  universo (SRU), ou seja, aplica as rota��es,
//  escalas e transla��es a um ponto no sistema
//  de refer�ncia do objeto SRO.
// ***********************************************
void InstanciaPonto(Ponto p, Ponto &out)
{
    GLfloat ponto_novo[4];
    GLfloat matriz_gl[4][4];
    int  i;

    glGetFloatv(GL_MODELVIEW_MATRIX,&matriz_gl[0][0]);

    for(i=0; i<4; i++)
    {
        ponto_novo[i] = matriz_gl[0][i] * p.x +
                        matriz_gl[1][i] * p.y +
                        matriz_gl[2][i] * p.z +
                        matriz_gl[3][i];
    }
    out.x = ponto_novo[0];
    out.y = ponto_novo[1];
    out.z = ponto_novo[2];

    //imprimeMatrizGL();

}
// **********************************************************************
//
//
// **********************************************************************
void GuardaCoodenadasDoVeiculo()
{

}
// **********************************************************************
//  void init(void)
//  Inicializa os par�metros globais de OpenGL
//
// **********************************************************************
void init(void)
{
    // Define a cor do fundo da tela (AZUL)
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    srand(unsigned(time(NULL)));
    for(int i=0; i<MAX; i++)
        //Linhas[i].geraLinha();
        Linhas[i].geraLinhaCurta();
    // Define as coordenadas da linha que representa o ve�culo
    Veiculo.x1 = 0;
    Veiculo.y1 = 1.0;
    Veiculo.x2 = 0;
    Veiculo.y2 = -1.0;

    // Define a posi��o inicial da linha que representa o ve�culo
    tx = 5;
    ty = 5;
    alfa = 0.0;

}

// **********************************************************************
//  void reshape( int w, int h )
//  trata o redimensionamento da janela OpenGL
//
// **********************************************************************
void reshape( int w, int h )
{
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Define os limites l�gicos da �rea OpenGL dentro da Janela
    glOrtho(0,10,0,10,0,1);

    // Define a �rea a ser ocupada pela �rea OpenGL dentro da Janela
    glViewport(0, 0, w, h);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

/* ********************************************************************** */
/*                                                                        */
/*  Calcula a interseccao entre 2 retas (no plano "XY" Z = 0)             */
/*                                                                        */
/* k : ponto inicial da reta 1                                            */
/* l : ponto final da reta 1                                              */
/* m : ponto inicial da reta 2                                            */
/* n : ponto final da reta 2                                              */
/*                                                                        */
/* s: valor do par�metro no ponto de interse��o (sobre a reta KL)         */
/* t: valor do par�metro no ponto de interse��o (sobre a reta MN)         */
/*                                                                        */
/* ********************************************************************** */
int intersec2d(Ponto k, Ponto l, Ponto m, Ponto n, double &s, double &t)
{
    double det;

    det = (n.x - m.x) * (l.y - k.y)  -  (n.y - m.y) * (l.x - k.x);

    if (det == 0.0)
        return 0 ; // n�o h� intersec��o

    s = ((n.x - m.x) * (m.y - k.y) - (n.y - m.y) * (m.x - k.x))/ det ;
    t = ((l.x - k.x) * (m.y - k.y) - (l.y - k.y) * (m.x - k.x))/ det ;

    return 1; // h� intersec��o
}

bool HaInterseccao(Ponto k, Ponto l, Ponto m, Ponto n)
{
    int ret;
    double s,t;
    ret = intersec2d( k,  l,  m,  n, s, t);
    if (!ret) return false;
    if (s>=0.0 && s <=1.0 && t>=0.0 && t<=1.0)
        return true;
    else return false;

}
void Redesenha(int i)
{
    glutPostRedisplay();
}
void DesenhaCenario()
{
    Ponto P1, P2, PA, PB, temp;
    // Calcula e armazena as coordenadas da linha que representa o "ve�culo"
    glPushMatrix();
    {
        glTranslatef(tx, ty, 0);
        glRotatef(alfa,0,0,1);
        // guarda as coordenadas do primeiro ponto da linha
        temp.set(Veiculo.x1, Veiculo.y1);
        InstanciaPonto(temp, P1);
        temp.set(Veiculo.x2, Veiculo.y2);
        InstanciaPonto(temp, P2);
    }
    glPopMatrix();

    // Desenha as linhas do cen�rio
    glLineWidth(1);
    glColor3f(1,1,0);

    for(int i=0; i<MAX; i++)
    {
        if (devoTestar)   // Esta vari�vel � controlada pela "tecla de espa�o"
        {
            temp.set(Linhas[i].x1, Linhas[i].y1);
            InstanciaPonto(temp, PA);
            temp.set(Linhas[i].x2, Linhas[i].y2);
            InstanciaPonto(temp, PB);
            if (HaInterseccao(PA, PB, P1, P2))
                glColor3f(1,0,0);
            else glColor3f(0,1,0);
        }
        else glColor3f(0,1,0);
        if (devoExibir) // Esta vari�vel � controlada pela 'e'
            Linhas[i].desenhaLinha();
    }

    // Desenha o ve�culo de novo
    glColor3f(1,0,1);
    glLineWidth(3);
    glPushMatrix();
    {
        glTranslatef(tx, ty, 0);
        glRotatef(alfa,0,0,1);
        Veiculo.desenhaLinha();
    }
    glPopMatrix();

}
// **********************************************************************
//  void display( void )
//
// **********************************************************************
void display( void )
{
    float new_time,base_time;
    // Limpa a tela com  a cor de fundo
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

#define QTD_FRAMES 500.0

    if (devoImprimirFPS) // Pressione f para imprimir
    {
        base_time = glutGet(GLUT_ELAPSED_TIME);

        for (int i=0; i< QTD_FRAMES;i++) // Repete o desenho do cenario
            DesenhaCenario();
        new_time = glutGet(GLUT_ELAPSED_TIME);

        float fps;
        fps = QTD_FRAMES/(new_time - base_time);

        cout << fps << " FPS." << endl;

        devoImprimirFPS = false;
    }
    else DesenhaCenario();

    glutSwapBuffers();

}


// **********************************************************************
//  void keyboard ( unsigned char key, int x, int y )
//
// **********************************************************************

void keyboard ( unsigned char key, int x, int y )
{

    switch ( key )
    {
    case 27:        // Termina o programa qdo
        exit ( 0 );   // a tecla ESC for pressionada
        break;
    case 'e':
        devoExibir = !devoExibir;
        break;
    case'r':
        alfa = alfa + 3;
        break;
    case'R':
        alfa = alfa - 3;
        break;
    case' ':
        devoTestar = !devoTestar;
        if (devoTestar)
            cout << "Interseccao LIGADA." << endl;
        else cout << "Interseccao DESLIGADA." << endl;
        break;
    case 'f':
        devoImprimirFPS = true;
        cout << "Comecou a contar..." << endl;
        break;
    default:
        break;
    }
    glutPostRedisplay();
}


// **********************************************************************
//  void arrow_keys ( int a_keys, int x, int y )
//
//
// **********************************************************************
void arrow_keys ( int a_keys, int x, int y )
{

    float incremento=0.2;

    switch ( a_keys )
    {
    case GLUT_KEY_UP:       // Se pressionar UP
        ty += incremento;
        break;
    case GLUT_KEY_DOWN:     // Se pressionar DOWN
        ty -= incremento;
        break;
    case GLUT_KEY_LEFT:       // Se pressionar LEFT
        tx -= incremento;
        break;
    case GLUT_KEY_RIGHT:     // Se pressionar RIGHT
        tx += incremento;
        break;
    default:
        break;
    }
    glutPostRedisplay();
}
void mouse(int button, int state, int x, int y)
{
    glutPostRedisplay();
}
void mouseMove(int x, int y)
{
    glutPostRedisplay();
}
// **********************************************************************
//  void main ( int argc, char** argv )
//
//
// **********************************************************************
int  main ( int argc, char** argv )
{
    glutInit            ( &argc, argv );
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
    glutInitWindowPosition (0,0);

    // Define o tamanho inicial da janela grafica do programa
    glutInitWindowSize  ( 650, 500);

    // Cria a janela na tela, definindo o nome da
    // que aparecera na barra de t�tulo da janela.
    glutCreateWindow    ( "C�lculo de colis�o." );

    // executa algumas inicializa��es
    init ();


    // Define que o tratador de evento para
    // o redesenho da tela. A funcao "display"
    // ser� chamada automaticamente quando
    // for necess�rio redesenhar a janela
    glutDisplayFunc ( display );
    glutIdleFunc ( display );

    // Define que o tratador de evento para
    // o redimensionamento da janela. A funcao "reshape"
    // ser� chamada automaticamente quando
    // o usu�rio alterar o tamanho da janela
    glutReshapeFunc ( reshape );

    // Define que o tratador de evento para
    // as teclas. A funcao "keyboard"
    // ser� chamada automaticamente sempre
    // o usu�rio pressionar uma tecla comum
    glutKeyboardFunc ( keyboard );

    // Define que o tratador de evento para
    // as teclas especiais(F1, F2,... ALT-A,
    // ALT-B, Teclas de Seta, ...).
    // A funcao "arrow_keys" ser� chamada
    // automaticamente sempre o usu�rio
    // pressionar uma tecla especial
    glutSpecialFunc ( arrow_keys );
    //glutMouseFunc(mouse);
    //glutMotionFunc(mouseMove);

    PrintMenu();
    // inicia o tratamento dos eventos
    glutMainLoop ( );

    return 0;
}

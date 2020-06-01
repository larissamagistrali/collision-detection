//------------------------------------------------
// Exercício sobre Aceleração de Testes de Colisao
// Larissa Magistrali, Nikolas Lacerda
//------------------------------------------------
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
#include <vector>
//------------------------------------------------------

//VARIAVEIS
const int MAX = 1500;
bool devoTestar = true;
bool devoExibir = true;
bool devoImprimirFPS = false;
Linha Linhas[MAX];
Linha Veiculo;
float tx, ty, alfa;
float glOrthoX=10,glOrthoY=10;

//MENU
void PrintMenu()
{
    cout << "-------------------MENU-------------------" << endl;
    cout << "-> e - esconde linhas." << endl;
    cout << "-> f - imprime FPS." << endl;
    cout << "-> r/R - rota veiculo." << endl;
    cout << "-> setas - movimenta veiculo." << endl;
    cout << "-> ESPACO - liga/desliga teste de colisao." << endl;
    cout << "------------------------------------------" << endl;
}

//MATRIZ DE TRANFOMACAO
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

//PONTO
void InstanciaPonto(Ponto p, Ponto &out)
{
    //  Esta função calcula as coordenadas
    //  de um ponto no sistema de referência do
    //  universo (SRU), ou seja, aplica as rotações,
    //  escalas e translações a um ponto no sistema
    //  de referência do objeto SRO.
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

//PARTICOES
typedef struct Particao{
    float xmin,xmax,ymin,ymax;
    std::vector<int> indices;
};

const int numParticoes=2.0; //4x4=16 particoes
Particao Particoes[numParticoes][numParticoes];

float larguraParticao = glOrthoX/numParticoes;
float alturaParticao  = glOrthoY/numParticoes;

float contx,conty=0.0;
int i,j=0;

void GeraParticoes(){
    for(i = 0; i<numParticoes; i++) {
        for(j = 0; j<numParticoes; j++){
            vector<int> indicesLinhasDaParticao;
            Particoes[i][j].xmin=contx;
            Particoes[i][j].ymin=conty;
            Particoes[i][j].xmax=contx+larguraParticao;
            Particoes[i][j].ymax=conty+alturaParticao;

            contx = contx + larguraParticao;

            for(int k=0; k<MAX; k++){
                if((Linhas[k].x1 >= Particoes[i][j].xmin && Linhas[k].x1 <= Particoes[i][j].xmax && Linhas[k].y1 >= Particoes[i][j].ymin && Linhas[k].y1 <= Particoes[i][j].ymax) ||
                   (Linhas[k].x2 >= Particoes[i][j].xmin && Linhas[k].x2 <= Particoes[i][j].xmax && Linhas[k].y2 >= Particoes[i][j].ymin && Linhas[k].y2 <= Particoes[i][j].ymax)){
                    indicesLinhasDaParticao.push_back(k);
                }
            }
            Particoes[i][j].indices=indicesLinhasDaParticao;
        }
        contx=0.0;
        conty=conty+alturaParticao;
    }
}

//INTERSECCAO
int intersec2d(Ponto k, Ponto l, Ponto m, Ponto n, double &s, double &t)
{
    //  Calcula a interseccao entre 2 retas (no plano "XY" Z = 0)

    // k : ponto inicial da reta 1
    // l : ponto final da reta 1
    // m : ponto inicial da reta 2
    // n : ponto final da reta 2

    // s: valor do parâmetro no ponto de interseção (sobre a reta KL)
    // t: valor do parâmetro no ponto de interseção (sobre a reta MN)

    double det;
    det = (n.x - m.x) * (l.y - k.y)  -  (n.y - m.y) * (l.x - k.x);
    if (det == 0.0)
        return 0 ; // não há intersecção
    s = ((n.x - m.x) * (m.y - k.y) - (n.y - m.y) * (m.x - k.x))/ det ;
    t = ((l.x - k.x) * (m.y - k.y) - (l.y - k.y) * (m.x - k.x))/ det ;
    return 1; // há intersecção
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

//DESENHOS
void Redesenha(int i)
{
    glutPostRedisplay();
}
void DesenhaCenario()
{
    Ponto P1, P2, PA, PB, temp;
    // Calcula e armazena as coordenadas da linha que representa o "veículo"
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

    // Desenha as linhas do cenário
    glLineWidth(1);
    glColor3f(1,1,0);

    //printf("VEICULO X1 %f Y1 %f\n", Veiculo.x1, Veiculo.y1);
    //printf("VEICULO X1 %f Y1 %f\n", Veiculo.x2, Veiculo.y2);
    //printf("VEICULO tx %f ty %f\n", tx, ty);

    if(devoExibir){
        for(int i=0; i<MAX; i++){
            glColor3f(0,1,0);
            Linhas[i].desenhaLinha();
        }
    }

    if(devoTestar){
        for(int i=0; i<numParticoes; i++){
            for(int j=0; j<numParticoes; j++){
                if((P2.x >= Particoes[i][j].xmin && P2.x <= Particoes[i][j].xmax && P2.y >= Particoes[i][j].ymin && P2.y <= Particoes[i][j].ymax)||
                   (P1.x >= Particoes[i][j].xmin && P1.x <= Particoes[i][j].xmax && P1.y >= Particoes[i][j].ymin && P1.y <= Particoes[i][j].ymax)){
                    //CASO O VEICULO ESTEJA NESSE SUBESPAÇO
                    printf("MOTORISTA NO QUADRANTE I  %d J %d\n", i, j);
                    for(int k=0; k<Particoes[i][j].indices.size(); k++){
                            temp.set(Linhas[Particoes[i][j].indices[k]].x1, Linhas[Particoes[i][j].indices[k]].y1);
                            InstanciaPonto(temp, PA);
                            temp.set(Linhas[Particoes[i][j].indices[k]].x2, Linhas[Particoes[i][j].indices[k]].y2);
                            InstanciaPonto(temp, PB);
                            if (HaInterseccao(PA, PB, P1, P2)){
                                if(devoExibir){
                                    glColor3f(1,0,0);
                                    Linhas[Particoes[i][j].indices[k]].desenhaLinha();
                                }
                            }
                    }
                }
            }
        }
    }
    // Desenha o veículo de novo
    glColor3f(1,0,1);
    glLineWidth(1);
    glPushMatrix();
    {
        glTranslatef(tx, ty, 0);
        glRotatef(alfa,0,0,1);
        Veiculo.desenhaLinha();
    }
    glPopMatrix();


    for(int i=0; i<numParticoes; i++){
        for(int j=0; j<numParticoes; j++){
            // Cada Quadrante
            glColor3f(0,0,1);
            glLineWidth(1);
            glBegin(GL_LINE_LOOP);
            glVertex2f(Particoes[i][j].xmin, Particoes[i][j].ymin);
            glVertex2f(Particoes[i][j].xmax, Particoes[i][j].ymin);
            glVertex2f(Particoes[i][j].xmax, Particoes[i][j].ymax);
            glVertex2f(Particoes[i][j].xmin, Particoes[i][j].ymax);
            glEnd();
        }
    }



    }

//DISPLAY
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

//COMANDOS
void keyboard ( unsigned char key, int x, int y )
{

    switch ( key )
    {
    case 27:          // Termina o programa qdo
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
void arrow_keys ( int a_keys, int x, int y )
{
    float incremento=0.1;
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

//INIT
void init(void)
{
    // Define a cor do fundo da tela (AZUL)
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    srand(unsigned(time(NULL)));
    for(int i=0; i<MAX; i++)
        //Linhas[i].geraLinha();
        Linhas[i].geraLinhaCurta();
    // Define as coordenadas da linha que representa o veículo
    Veiculo.x1 = 0;
    Veiculo.y1 = 1.0;
    Veiculo.x2 = 0;
    Veiculo.y2 = -1.0;
    // Define a posição inicial da linha que representa o veículo
    tx = 5;
    ty = 5;
    alfa = 0.0;

    GeraParticoes();


    for(int i = 0; i<numParticoes; i++) {
        for(int j = 0; j<numParticoes; j++){
            printf("I %d J %d\n", i, j);
            printf("linhas = %d\n", Particoes[i][j].indices.size());
        }
    }
}

//RESHAPE
void reshape( int w, int h )
{
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Define os limites lógicos da área OpenGL dentro da Janela
    glOrtho(0,glOrthoX,0,glOrthoY,0,1);
    // Define a área a ser ocupada pela área OpenGL dentro da Janela
    glViewport(0, 0, w, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//MAIN
int  main ( int argc, char** argv )
{
    glutInit            ( &argc, argv );
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
    glutInitWindowPosition (0,0);
    // Define o tamanho inicial da janela grafica do programa
    glutInitWindowSize  ( 650, 500);
    // Cria a janela na tela, definindo o nome da
    // que aparecera na barra de título da janela.
    glutCreateWindow    ( "Calculo de colisao." );
    // executa algumas inicializações
    init ();
    // Define que o tratador de evento para
    // o redesenho da tela. A funcao "display"
    // será chamada automaticamente quando
    // for necessário redesenhar a janela
    glutDisplayFunc ( display );
    glutIdleFunc ( display );
    // Define que o tratador de evento para
    // o redimensionamento da janela. A funcao "reshape"
    // será chamada automaticamente quando
    // o usuário alterar o tamanho da janela
    glutReshapeFunc ( reshape );
    // Define que o tratador de evento para
    // as teclas. A funcao "keyboard"
    // será chamada automaticamente sempre
    // o usuário pressionar uma tecla comum
    glutKeyboardFunc ( keyboard );
    // Define que o tratador de evento para
    // as teclas especiais(F1, F2,... ALT-A,
    // ALT-B, Teclas de Seta, ...).
    // A funcao "arrow_keys" será chamada
    // automaticamente sempre o usuário
    // pressionar uma tecla especial
    glutSpecialFunc ( arrow_keys );
    //glutMouseFunc(mouse);
    //glutMotionFunc(mouseMove);
    PrintMenu();
    // inicia o tratamento dos eventos
    glutMainLoop ( );
    return 0;
}

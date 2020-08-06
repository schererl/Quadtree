#include <iostream>

using namespace std;
int Cintersec2d(Ponto k, Ponto l, Ponto m, Ponto n, double &s, double &t);
bool CHaInterseccao(Ponto k, Ponto l, Ponto m, Ponto n);
void DrawSubdvision(Ponto initial, Ponto limit);
bool IntersecSubspace(Ponto a, Ponto b, Ponto c, Ponto d, Ponto r1, Ponto r2);

struct Node
{
   int *indice_linhas;
   int numero_linhas;
   int info;
   Node(int numero_linhas)
   {
        info = 1; //0 = empty  / 1 = content / 2 = atom
        this->numero_linhas = numero_linhas;
   }
};

class Quadtree
{
    Node *instance;

    Ponto origin;
    Ponto limit;
    Linha* lines;
    Quadtree *div_1;
    Quadtree *div_2;
    Quadtree *div_3;
    Quadtree *div_4;

    int altura;
    int max_altura;
    int objects_limit;
    int size_lines;


public:
    Quadtree(int objects_limit, int max_altura, int altura, int size_lines, Linha *lines, Ponto origin, Ponto limit)
    {
        div_1 = NULL;
        div_2 = NULL;
        div_1 = NULL;
        div_2 = NULL;
        instance = NULL;

        this -> objects_limit = objects_limit;
        this -> origin = origin;
        this -> limit = limit;
        this-> lines = lines;
        this-> size_lines = size_lines;
        this-> max_altura = max_altura;
        this-> altura = altura;
    }
    Quadtree()
    {

    }

    void Insert(Node *);
    void Subdivide(float initial_x, float initial_y, float limit_x, float limit_y, int op);
    void Search(Ponto p1, Ponto p2);
    int  VerifyLines(Ponto novo_inicio, Ponto novo_limite);
    void PopulaLinhas(Node *nodo);
};

void Quadtree::Insert(Node *node)
{

    (this -> instance) = node;
    //não existem linhas no quadrante
    if (node -> numero_linhas  == 0)
    {
        node->info = 0;
        return;
    }

    //se o quadrante não excede o número máximo de linhas, não subdivide
    if( this->max_altura == this->altura || this -> instance -> numero_linhas <= objects_limit)
    {
        PopulaLinhas(node);
        node->info = 2;
        return;
    }
    else
    {
        Subdivide(origin.x, (limit.y + origin.y)/2, (origin.x+limit.x)/2, limit.y, 1);

        Subdivide((origin.x + limit.x)/2, (limit.y + origin.y)/2, limit.x, limit.y, 2);

        Subdivide(origin.x, origin.y, (origin.x + limit.x)/2, (origin.y + limit.y)/2, 3);

        Subdivide((limit.x + origin.x)/2, origin.y, limit.x, (origin.y + limit.y)/2, 4);
    }
}

void Quadtree::Subdivide(float initial_x, float initial_y, float limit_x, float limit_y, int op)
{

    Ponto new_initial, new_limit;
    new_initial.set(initial_x, initial_y);
    new_limit.set(limit_x, limit_y);
    int indices = VerifyLines(new_initial, new_limit); //busca pelas linhas presentes no subespaço
    DrawSubdvision(new_initial, new_limit);
    if(op == 1)
    {
        div_1 = new Quadtree(this -> objects_limit, this->max_altura, this->altura+1, this->size_lines,  this-> lines, new_initial, new_limit);
        div_1 -> Insert(new Node(indices));
    }
    else if(op == 2)
    {
        div_2 = new Quadtree(this -> objects_limit, this->max_altura, this->altura+1, this->size_lines, this -> lines, new_initial, new_limit);
        div_2 -> Insert(new Node(indices));
    }
    else if(op == 3)
    {
        div_3 = new Quadtree(this -> objects_limit,this->max_altura, this->altura+1, this->size_lines, this -> lines, new_initial, new_limit);
        div_3 -> Insert(new Node(indices));
    }
    else if(op == 4)
    {
        div_4 = new Quadtree(this -> objects_limit, this->max_altura, this->altura+1, this->size_lines, this -> lines, new_initial, new_limit);
        div_4 -> Insert(new Node(indices));
    }



}

void Quadtree::Search(Ponto p1, Ponto p2) //testar linha
{

    Ponto baixo_esq, baixo_dir, cima_esq, cima_dir;
    bool inside;

    if(this->instance->info == 0)
    {
        return;
    }

    if(this->instance->info == 2) {
        //cout << "QUADRANTE COM " << this->instance->numero_linhas << " LINHAS" << endl;
        glPushMatrix();
        
        glColor3f(225,0,0);
        
        for(int i = 0; i < this->instance->numero_linhas;i++)
        {
            Linha *tmp = &this->lines[this->instance->indice_linhas[i]];
            Ponto a, b;
            a.set(tmp->x1, tmp->y1);
            b.set(tmp->x2, tmp->y2);
            if(CHaInterseccao(p1, p2, a ,b))
            {
                tmp->desenhaLinha();
            }
        }
        
        DrawSubdvision(origin, limit);
        glPopMatrix();
        return;
    }
    else
    {
        DrawSubdvision(origin, limit);
        
    }
    

    //cima esq
    baixo_esq.set(origin.x, (origin.y+limit.y)/2);
    cima_dir.set((origin.x + limit.x)/2, limit.y);
    baixo_dir.set((origin.x + limit.x)/2, (origin.y+limit.y)/2);
    cima_esq.set(origin.x, limit.y);

    inside = (origin.x <= p1.x
    && (limit.y + origin.y)/2 <=p1.y
    && (origin.x+limit.x)/2 >= p1.x
    && limit.y >= p1.y)
    ||
    (origin.x <= p2.x
    && (limit.y + origin.y)/2 <= p2.y
    && (origin.x+limit.x)/2 >= p2.x
    && limit.y >= p2.y);

    if(IntersecSubspace(baixo_esq, baixo_dir, cima_dir, cima_esq, p1, p2) || inside)
    {
        if(this->div_1 != NULL)
            this->div_1->Search(p1, p2);

    }

    //cima dir
    baixo_esq.set((origin.x +limit.x)/2, (origin.y + limit.y)/2);
    cima_dir.set(limit.x, limit.y);
    baixo_dir.set(limit.x, (origin.y + limit.y)/2);
    cima_esq.set((origin.x +limit.x)/2, limit.y);

    inside = ((origin.x +limit.x)/2 <= p1.x
    && (origin.y + limit.y)/2 <=p1.y
    && limit.x >= p1.x
    && limit.y >= p1.y)
    ||
    ((origin.x +limit.x)/2 <= p2.x
    && (origin.y + limit.y)/2 <=p2.y
    && limit.x >= p2.x
    && limit.y >= p2.y);
    if(IntersecSubspace(baixo_esq, baixo_dir, cima_dir, cima_esq, p1, p2) || inside)
    {
        if(this->div_2 != NULL)
            this->div_2->Search(p1, p2);


    }

    //baixo esq
    baixo_esq.set(origin.x, origin.y);
    cima_dir.set((origin.x + limit.x)/2, (origin.y + limit.y)/2);
    baixo_dir.set((origin.x + limit.x)/2, origin.y);
    cima_esq.set(origin.x, (origin.y + limit.y)/2);


    inside = (origin.x <= p1.x
    && origin.y <=p1.y
    && (origin.x+limit.x)/2 >= p1.x
    && (origin.y + limit.y)/2 >= p1.y)
    ||
    (origin.x <= p2.x
    && origin.y <= p2.y
    && (origin.x+limit.x)/2 >= p2.x
    && (origin.y + limit.y)/2 >= p2.y);
    if(IntersecSubspace(baixo_esq, baixo_dir, cima_dir, cima_esq, p1, p2) || inside)
    {
        if(this->div_3 != NULL)
           this->div_3->Search(p1, p2);
    }

    //baixo dir
    baixo_esq.set((origin.x + limit.x)/2, origin.y);
    cima_dir.set(limit.x, (origin.y + limit.y)/2);
    baixo_dir.set(limit.x, origin.y);
    cima_esq.set((origin.x + limit.x)/2, (origin.y + limit.y)/2);

    inside = ((origin.x +limit.x)/2 <= p1.x
    && origin.y <=p1.y
    && limit.x >= p1.x
    && (origin.y + limit.y)/2 >= p1.y)
    ||
    ((origin.x +limit.x)/2 <= p2.x
    && origin.y <=p2.y
    && limit.x >= p2.x
    && (origin.y + limit.y)/2 >= p2.y);

    if(IntersecSubspace(baixo_esq, baixo_dir, cima_dir, cima_esq, p1, p2) || inside)
    {
        if(this->div_4 != NULL)
            this->div_4->Search(p1, p2);
    }


}

//insere os índices das linhas dentro do subespaço folha
void Quadtree::PopulaLinhas(Node *nodo)
{

    int counter = 0; //número de linhas dentro do subespaço
    nodo->indice_linhas = (int *) malloc(nodo->numero_linhas * sizeof(int));
    for(int i = 0; i < this->size_lines;i++)
    {

        //verifica se o origem ou limit está dentro do quadrante
        if ( (this->lines[i].x1 >= this->origin.x && this->lines[i].x1 <= this->limit.x && this->lines[i].y1 >= this->origin.y && this->lines[i].y1 <= this->limit.y ) ||
            (this->lines[i].x2 >= this->origin.x && this->lines[i].x2 <= this->limit.x && this->lines[i].y2 >= this->origin.y && this->lines[i].y2 <= this->limit.y) )
        {
            nodo->indice_linhas[counter] = i;
            counter++;
            continue;
        }


        //verifica se a linha está dentro do quadrante
        Ponto esq_inf, dir_inf, esq_sup, dir_sup, line_ori, line_limit;
        line_ori.set(this->lines[i].x1, this->lines[i].y1);
        line_limit.set(this->lines[i].x2, this->lines[i].y2);
        esq_inf.set(this->origin.x, this->origin.y);
        dir_inf.set(this->limit.x, this->origin.y);
        esq_sup.set(this->origin.x, this->limit.y);
        dir_sup.set(this->limit.x, this->limit.y);

        bool baixo = CHaInterseccao(esq_inf, dir_inf, line_ori, line_limit);
        bool cima = CHaInterseccao(esq_sup, dir_sup, line_ori, line_limit);
        bool esquerda = CHaInterseccao(esq_inf, esq_sup, line_ori, line_limit);
        bool direita = CHaInterseccao(dir_inf, dir_sup, line_ori, line_limit);

       if( baixo || cima || esquerda || direita)
        {
            nodo->indice_linhas[counter] = i;
            counter++;
        }
    }
}

//verifica o número de linhas dentro de um subespaço
int Quadtree::VerifyLines(Ponto novo_inicio, Ponto novo_limite)
{
    int counter = 0; //número de linhas dentro do subespaço
    for(int i = 0; i < this->size_lines;i++)
    {

        //verifica se o origem ou limit está dentro do quadrante
        if ( (this->lines[i].x1 >= novo_inicio.x && this->lines[i].x1 <= novo_limite.x && this->lines[i].y1 >= novo_inicio.y && this->lines[i].y1 <= novo_limite.y ) ||
            (this->lines[i].x2 >= novo_inicio.x && this->lines[i].x2 <= novo_limite.x && this->lines[i].y2 >= novo_inicio.y && this->lines[i].y2 <= novo_limite.y) )
        {
            counter++;
            continue;
        }


        //verifica se a linha está dentro do quadrante
        Ponto esq_inf, dir_inf, esq_sup, dir_sup, line_ori, line_limit;
        line_ori.set(this->lines[i].x1, this->lines[i].y1);
        line_limit.set(this->lines[i].x2, this->lines[i].y2);
        esq_inf.set(novo_inicio.x, novo_inicio.y);
        dir_inf.set(novo_limite.x, novo_inicio.y);
        esq_sup.set(novo_inicio.x, novo_limite.y);
        dir_sup.set(novo_limite.x, novo_limite.y);

        bool baixo = CHaInterseccao(esq_inf, dir_inf, line_ori, line_limit);
        bool cima = CHaInterseccao(esq_sup, dir_sup, line_ori, line_limit);
        bool esquerda = CHaInterseccao(esq_inf, esq_sup, line_ori, line_limit);
        bool direita = CHaInterseccao(dir_inf, dir_sup, line_ori, line_limit);

       if( baixo || cima || esquerda || direita) counter++;
    }
    return counter;
}

bool IntersecSubspace(Ponto a, Ponto b, Ponto c, Ponto d, Ponto r1, Ponto r2)
{
    return CHaInterseccao(a, b, r1, r2) || CHaInterseccao(b, c, r1, r2) || CHaInterseccao(c, d, r1, r2) || CHaInterseccao(d, a, r1, r2);
}

//código pronto do projeto, criar uma classe estatíca para usar eles
int Cintersec2d(Ponto k, Ponto l, Ponto m, Ponto n, double &s, double &t)
{
    double det;

    det = (n.x - m.x) * (l.y - k.y)  -  (n.y - m.y) * (l.x - k.x);

    if (det == 0.0)
        return 0 ; // não há intersecção

    s = ((n.x - m.x) * (m.y - k.y) - (n.y - m.y) * (m.x - k.x))/ det ;
    t = ((l.x - k.x) * (m.y - k.y) - (l.y - k.y) * (m.x - k.x))/ det ;

    return 1; // há intersecção
}

bool CHaInterseccao(Ponto k, Ponto l, Ponto m, Ponto n)
{
    int ret;
    double s,t;
    ret = Cintersec2d( k,  l,  m,  n, s, t);
    if (!ret) return false;
    if (s>=0.0 && s <=1.0 && t>=0.0 && t<=1.0)
        return true;
    else return false;

}


void DrawSubdvision(Ponto initial, Ponto limit)
{
    glPushMatrix();
    glColor3f(0,0,0);
	glBegin(GL_LINES);
		glVertex2f(initial.x,initial.y);
		glVertex2f(initial.x,limit.y);

        glVertex2f(initial.x,limit.y);
		glVertex2f(limit.x,limit.y);

        glVertex2f(limit.x,limit.y);
		glVertex2f(limit.x,initial.y);

		glVertex2f(limit.x,initial.y);
        glVertex2f(initial.x,initial.y);
    glEnd();
    glPopMatrix();
}



#include<stdio.h>

typedef struct node {
    char ch[2];
    int weight;
    char ldata, rdata;
    struct node* lchild;
    struct node* rchild;
    struct node* parent;
} huffman_node;

typedef struct encoding {
    char ch[2];
    char* data;
} encode1;

void paixu(huffman_node** p, int m) {
    huffman_node* temp;
    for (int i = 0; i < m - 1; i++) {
        for (int j = i + 1; j < m; j++) {
            if (p[i]->weight > p[j]->weight) {
                //�������ָ��ʵ������
                temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }
}

//���1�����Ľ���
huffman_node** Read(int n) {
    FILE* fp;
    huffman_node** huff;
    //��������2*n-1�����ָ�������
    huff = (huffman_node**)malloc((2 * n - 1) * sizeof(huffman_node*));
    for (int i = 0; i < 2 * n - 1; i++) {
        huff[i] = (huffman_node*)malloc(sizeof(huffman_node));
        huff[i]->lchild = huff[i]->rchild = NULL;
    }
    if ((fp = fopen("hfmTree.txt", "w")) == NULL) {
        printf("�ļ���ʧ��!");
        exit(0);
    }
    printf("��������ĸ��Ȩֵ����ĸ��Ȩ�أ�:\n");
    for (int i = 0; i < n; i++) {
        scanf("%s %d", huff[i]->ch, &huff[i]->weight);
        fgetc(stdin);  // consume newline
        fprintf(fp, "%s %d ", huff[i]->ch, huff[i]->weight);
    }
    printf("hfmTree.txt�ļ�д�����!\n");
    fclose(fp);
    return huff;
}

//
huffman_node* Crhuffman(huffman_node** p, int n, int* n1) {
    int m = n;
    for (int i = 0; i < 2 * n - 1; i += 2) {
        if ((m - 1) != i) {
            paixu(p, m);
            p[m]->weight = p[i]->weight + p[i + 1]->weight;
            p[m]->lchild = p[i];
            p[m]->rchild = p[i + 1];
            p[i]->parent = p[m];
            p[i + 1]->parent = p[m];
            m += 1;
        } else {
            p[m - 1]->parent = NULL;
            break;
        }
    }
    *n1 = m;
    return p[m - 1];
}

//����һ
int Readtxt(char* c) {
    int m = 0;
    FILE* fp;
    printf("��������Ҫ�����Ӣ��:\n");
    if ((fp = fopen("ToBeTran.txt", "w")) == NULL) {
        printf("�ļ���ʧ��!\n");
        exit(0);
    }
    for (int i = 0;; i++) {
        c[i] = getchar();
        if (c[i] == '\n') {
            c[i] = '\0';
            break;
        }
        m += 1;
    }
    fprintf(fp, "%s", c);
    fclose(fp);
    printf("ToBeTran.txt�ļ�д�����!\n");
    return m;
}

//�����
int Code(huffman_node** p, int n, int t, char* c, int c1, char* c2) {
    huffman_node* q;
    FILE* fp;
    q = (huffman_node*)malloc(sizeof(huffman_node));
    int k = 0, k1 = t - 1, kk = 0;
    encode1* encode;
    encode = (encode1*)malloc(t * sizeof(encode1));
    for (int i = 0; i < t; i++) {
        encode[i].data = (char*)malloc(t * sizeof(char));
    }
    for (int i = 0; i < t; i++) {
        for (int j = 0; j < t; j++) {
            encode[i].data[j] = 'a';
        }
    }
    for (int i = 0; i < n; i++) {
        if (p[i]->lchild == NULL && p[i]->rchild == NULL) {
            q = p[i];
            encode[k].ch[0] = p[i]->ch[0];
            while (q->parent != NULL) {
                if (q->parent->lchild == q)
                    encode[k].data[k1] = '0';
                else
                    encode[k].data[k1] = '1';
                q = q->parent;
                k1 = k1 - 1;
            }
            k += 1;
            k1 = t - 1;
        }
    }
    if ((fp = fopen("CodeFile.txt", "w")) == NULL) {
        printf("�ļ���ʧ��!\n");
        exit(0);
    }
    for (int i = 0; i < t; i++) {
        encode[i].ch[1] = '\0';
        fprintf(fp, "%s:", encode[i].ch);
        for (int j = 0; j < t; j++) {
            if (encode[i].data[j] != 'a') {
                fprintf(fp, "%c", encode[i].data[j]);
                if (j == t - 1)
                    fprintf(fp, "\n");
            }
        }
    }
    for (int i = 0; i < c1; i++) {
        for (int j = 0; j < t; j++) {
            if (c[i] == encode[j].ch[0])
                for (int k2 = 0; k2 < t; k2++) {
                    if (encode[j].data[k2] != 'a') {
                        c2[kk] = encode[j].data[k2];
                        kk += 1;
                    }
                }
        }
    }
    c2[kk] = '\0';
    fprintf(fp, "��Ŀ����Ӣ�ı���: %s", c2);
    printf("CodeFile�ļ�д�����!\n");
    fclose(fp);
    return kk;
}

//����1���ʼ������
void Initialization(huffman_node*** p, huffman_node** p1, int* n, int* n1) {
    printf("�����������Ӣ����ĸ����:\n");
    scanf("%d", n);
    *p = Read(*n);
    *p1 = Crhuffman(*p, *n, n1);
}

//��ȡ�ļ�����������
void Encoding(int* c1, char** c, char** c2, int n, int n1, huffman_node** p) {
    *c1 = Readtxt(*c);
    Code(p, n1, n, *c, *c1, *c2);
}

//�����д���ļ�TextFile.txt
void Decoding(huffman_node* p1, char* c) {
    int k = 0, i = 0;
    FILE* fp;
    huffman_node* p = p1;
    if ((fp = fopen("TextFile.txt", "w")) == NULL) {
        printf("�ļ���ʧ��!\n");
        exit(0);
    }
    while (c[i] != '\0') {
        while (c[i] != '\0') {
            if (c[i] == '0') {
                p = p->lchild;
                if (p->lchild == NULL && p->rchild == NULL) {
                    i++;
                    break;
                }
            } else {
                p = p->rchild;
                if (p->lchild == NULL && p->rchild == NULL) {
                    i++;
                    break;
                }
            }
            i++;
        }
        fprintf(fp, "%s", p->ch);
        p = p1;
    }
    fclose(fp);
    printf("TextFile.txt�ļ�д�����!\n");
}

//��ӡ�ļ�
void Print() {
    FILE* fp, *fp1;
    char ch[1000];
    if ((fp = fopen("CodeFile.txt", "r")) == NULL) {
        printf("�ļ���ʧ��!\n");
        exit(0);
    }
    if ((fp1 = fopen("CodePrint.txt", "w")) == NULL) {
        printf("�ļ���ʧ��!\n");
        exit(0);
    }
    while (!feof(fp)) {
        fscanf(fp, "%s", ch);
        if (ch[0] == '#') {
            printf("%s\n", ch);
            fscanf(fp, "%s", ch);
            fprintf(fp1, "%s", ch);
        }
        printf("%s\n", ch);
    }
    fclose(fp);
    fclose(fp1);
}

//��ӡ��������
void Tree_printing(huffman_node* p1) {
    FILE* fp;
    if ((fp = fopen("TreePrint.txt", "a")) == NULL) {
        printf("�ļ���ʧ��!\n");
        exit(0);
    }
    if (p1) {
        if (p1->ch[0] == '\0') {
            printf("��%d    ", p1->weight);
            fprintf(fp, "��%d\n", p1->weight);
        } else {
            printf("%c%d     ", p1->ch[0], p1->weight);
            fprintf(fp, "%c%d\n", p1->ch[0], p1->weight);
        }

        Tree_printing(p1->lchild);
        Tree_printing(p1->rchild);
    }
}

//���˵�����
void homepage() {
    printf("****************************************************************************\n");
    printf("               1.Initialization And Encoding     //��ʼ��������             \n");
    printf("               2.Decoding                        //����                     \n");
    printf("               3.Print                           //��ӡ�����ļ�             \n");
    printf("               4.Tree_printing                   //��ӡ��������             \n");
    printf("               5.Confirm exit                    //ȷ���˳�                 \n");
    printf("****************************************************************************\n");
    printf("��ѡ����Ҫ�Ĳ�����\n");
}

int main(){
    int N;
    huffman_node** p;
    huffman_node* p1;
    encode1* q;
    char* c;
    char* c2;
    //���ַ������ڴ�ռ�
    c = (char*)malloc(100 * sizeof(char));
    c2 = (char*)malloc(1000 * sizeof(char));
    int n, n1, c1, n2;
    int i = 0; // Add this line to declare and initialize 'i'
    homepage();
    scanf("%d", &N);
    while (1) {
        // system("cls");
        if (N == 1) {
            Initialization(&p, &p1, &n, &n1);
            Encoding(&c1, &c, &c2, n, n1, p);
            i += 1;
        } else if (N == 2) {
            if (i == 0) {
                printf("�������Ϸ������������룡\n");
            } else {
                Decoding(p1, c2);
                i += 1;
            }
        } else if (N == 3) {
            if (i == 0) {
                printf("�������Ϸ������������룡\n");
            } else {
                printf("");
                Print();
                i += 1;
            }
        } else if (N == 4) {
            if (i == 0) {
                printf("�������Ϸ������������룡\n");
            } else {
                Tree_printing(p1);
                i += 1;
                printf("\n");
            }
        } else if (N == 5)
            exit(0);
        else
            printf("���벻�Ϸ�,���������룡\n");
        homepage();
        scanf("%d", &N);
    }
    return 0;
}

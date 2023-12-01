#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAXSIZE 100

typedef struct {
	char ch;//����/��ȥ
	int time_hour;//����ͣ��ʱ��
	int time_minute;
	int car;//�������ƺ�
}CarData;
//ջʵ��ͣ����
typedef struct {
	CarData data[MAXSIZE];//ͣ�������ͣ����
	int top;
}StopCar;

typedef struct WaitCar{
	CarData data[MAXSIZE];//�Ⱥ������ͣ����
	struct WaitCar* next;
}WaitCar;
typedef struct {
	WaitCar *front;
	WaitCar* rear;
}WaitCarLink;
//ջʵ�ָ���ʱ������������·
typedef struct {
	CarData data[MAXSIZE];
	int top;
}TemporaryCar;

void InitStopCar(StopCar *);//ͣ������ʼ��
int InitWaitCarLink(WaitCarLink*);//�Ⱥ�����ʼ��
void InitTemporaryCar(TemporaryCar*);//��ʱͣ����ʼ��

void ArrivalCar(StopCar*, WaitCarLink*, char, int, int, int);//��⺯��
void DepartureCar(StopCar*, char, int, int, int);//�뿪����
int IfTheCar(StopCar *,int);//�ж��Ƿ�������������
void WaitToStop(StopCar*, WaitCarLink*);//������ĳ����복��

int n, cost, j = 0, k = 0;//n���û�����ͣ����ͣ�ŵĳ���cost�Ƿ���
int main() {
	int car, time_hour, time_minute = 1;//timeͣ��ʱ��
	char ch = ' ';//����״̬
	char option[10];//��ѯ����
	StopCar sc;//ͣ����
	InitStopCar(&sc);
	WaitCarLink wcl;
	InitWaitCarLink(&wcl);
	WaitCar* wc;//�������ָ��

	printf("������ͣ��������ͣ�ŵ�������Լ�ͣ��ÿСʱ��ȡ�ķ��ã�Ԫ�����ÿո�������");
	scanf("%d %d", &n, &cost);
	while (1)
	{
		fflush(stdin);//�������������ö�ѭ��һ��
		printf("\n�����복��״̬(A���D��ȥ, C��ѯ�������ĳ��ƺţ�����ʱ�䣨����ʱ�䣬��ȥʱ�䣩��");
		scanf(" %c %d %d:%d", &ch, &car, &time_hour, &time_minute);//���ﻺ���������Ļ��з����±�������ⷨ��%cǰ�ӿո�
		//printf("ch=%c, car=%d, time=%d\n", ch, car, time);���һ�£���֤�����������Ƿ���
		/*if(car<10000||car>99999)
        {
            printf("���ƺŷǷ�");
            break;
        }*/
		if(time_hour>24||time_hour<0||time_minute>60||time_minute<0)
        {
            printf("ʱ����������ʱ������");
            break;
        }
		if (ch=='A')
		{
			ArrivalCar(&sc, &wcl, ch, car, time_hour, time_minute);
		}
		else if (ch=='D')
		{
			DepartureCar(&sc, ch, car, time_hour, time_minute);
			WaitToStop(&sc, &wcl);//ֱ�ӽ�����ϵĳ��Ƶ�����
		}
		else if (ch=='C')
        {
            printf("�Ƿ�����ѯ����(��/��)��");
            scanf("%s", option);
            if (strcmp(option, "��") == 0)
            {
                printf("�����ѯ�����С���\n");
                PrintAllCars(sc);
            }
        }
		else
		{
			printf("��������ȷ�ı�ţ�\n");
		}
	}
	return 0;
}
void WaitToStop(StopCar* sc, WaitCarLink* wcl)//������ϵĳ��Ƶ�����
{
	if (wcl->front == wcl->rear)//���û�г�
		printf("û�б������\n");
	else {
		//����
		WaitCar *S;
		S = wcl->front->next;//Sָ���ͷ
		wcl->front->next = S->next;//Sָ��Ķ�ͷ����
		if (wcl->rear == S)//���S�����һ��Ԫ�أ��ö�βָ��ָ���ͷ��ֹ���Ұָ��
			wcl->rear = wcl->front;
		printf("%d�����ɹ���\n", S->data[k].car);
		//��ջ
		sc->data[sc->top + 1].car = S->data[k].car;
		sc->data[sc->top + 1].ch = S->data[k].ch;
		sc->data[sc->top + 1].time_hour = S->data[k].time_hour;
		sc->data[sc->top + 1].time_minute = S->data[k].time_minute;
		printf("%d����ɹ���\n", S->data[k].car);
		k++;
		sc->top++;
	}
}
void DepartureCar(StopCar* sc, char ch, int car, int time_hour, int time_minute)//�뿪
{
	TemporaryCar tc;
	InitTemporaryCar(&tc);
	int timesum;
	if (sc->top == -1)
		printf("����û�г����޷��뿪\n");
	else if (IfTheCar(sc,car)) {
		printf("������û�д˳���\n");
	}
	else
	{
		while (sc->data[sc->top].car != car)//�ڴ˳�ǰ��ĳ�������
		{
			tc.data[tc.top + 1].car = sc->data[sc->top].car;
			tc.data[tc.top + 1].ch = sc->data[sc->top].ch;
			tc.data[tc.top + 1].time_hour = sc->data[sc->top].time_hour;
			tc.data[tc.top + 1].time_minute = sc->data[sc->top].time_minute;
			tc.top++;
			sc->top--;//�˳���ջ
		}
		if (time_hour < sc->data[sc->top].time_hour)
        {
            printf("ͣ������12Сʱ��������ͣ������������һ�찴һ���㣩��");
            int u = 0;
            scanf("%d",&u);
            time_hour+=24;
        }
		timesum = (time_hour - sc->data[sc->top].time_hour);
		if (time_minute - sc->data[sc->top].time_minute > 0)
        {
            timesum++;
        }
		printf("������%d��ͣ��ͣ����ʱ����%dСʱ,ͣ���ķ�����%dԪ\n", sc->data[sc->top].car, timesum,timesum * cost);
		sc->top--;
		while (tc.top != -1)//��ʱͣ��������ͣ����
		{
			sc->data[sc->top + 1].car = tc.data[tc.top].car;
			sc->data[sc->top + 1].ch = tc.data[tc.top].ch;
			sc->data[sc->top + 1].time_hour = tc.data[tc.top].time_hour;
			sc->data[sc->top + 1].time_minute = tc.data[tc.top].time_minute;
			tc.top--;
			sc->top++;
		}
	}
}
void PrintAllCars(StopCar* sc) {
    for (int i = 0; i <= sc->top; ++i) {
        printf("λ�� %d �ĳ��ƺ��ǣ�%d\n", i + 1, sc->data[i].car);
    }
}
void ArrivalCar(StopCar* sc, WaitCarLink* wcl, char ch, int car, int time_hour, int time_minute)//��⺯��
{

	WaitCar* wc;//�������ָ��
	wc = (WaitCar*)malloc(sizeof(WaitCar));
	wc->next = NULL;
	if ((sc->top + 1) < n) {//����û��
		sc->data[sc->top + 1].ch = ch;
		sc->data[sc->top + 1].car = car;
		sc->data[sc->top + 1].time_hour = time_hour;
		sc->data[sc->top + 1].time_minute = time_minute;
		sc->top++;
		printf("\n���ƺ�Ϊ%d�ĳ�ͣ��λ���ǣ�%d\n", car, sc->top + 1);
	}
	else//���˽����
	{
		wc->data[j].car = car;
		wc->data[j].ch = ch;
		wc->data[j].time_hour = time_hour;
		wc->data[j].time_minute = time_minute;
		wc->next = NULL;
		wcl->rear->next = wc;
		wcl->rear = wc;
		printf("���ƺ�Ϊ%d�ĳ��ڱ���ϵ�λ���ǣ�%d\n", car, j + 1);
		j++;
	}
}
int IfTheCar(StopCar *sc, int car)//�ж��Ƿ�������������
{
	int i = 0;
	while (i <= sc->top)
	{
		if (sc->data[i].car == car)
			return 0;
		i++;
	}
	return 1;
}
void InitStopCar(StopCar* sc)//ͣ������ʼ��
{
	sc->top = -1;
}
int InitWaitCarLink(WaitCarLink* wcl)//�Ⱥ�����ʼ��
{
	wcl->front = (WaitCar*)malloc(sizeof(WaitCar));
	if (wcl->front != NULL)
	{
		wcl->rear = wcl->front;
		wcl->front->next = NULL;
		return 0;
	}
	else
		return 1;
}
void InitTemporaryCar(TemporaryCar* tc)//��ʱͣ����ʼ��
{
	tc->top = -1;
}

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAXSIZE 100

typedef struct {
	char ch;//到达/离去
	int time_hour;//车辆停车时间
	int time_minute;
	int car;//车辆车牌号
}CarData;
//栈实现停车场
typedef struct {
	CarData data[MAXSIZE];//停车场最大停车量
	int top;
}StopCar;

typedef struct WaitCar{
	CarData data[MAXSIZE];//等候区最大停车量
	struct WaitCar* next;
}WaitCar;
typedef struct {
	WaitCar *front;
	WaitCar* rear;
}WaitCarLink;
//栈实现给临时出来的汽车让路
typedef struct {
	CarData data[MAXSIZE];
	int top;
}TemporaryCar;

void InitStopCar(StopCar *);//停车场初始化
int InitWaitCarLink(WaitCarLink*);//等候区初始化
void InitTemporaryCar(TemporaryCar*);//临时停车初始化

void ArrivalCar(StopCar*, WaitCarLink*, char, int, int, int);//入库函数
void DepartureCar(StopCar*, char, int, int, int);//离开函数
int IfTheCar(StopCar *,int);//判断是否有输入这辆车
void WaitToStop(StopCar*, WaitCarLink*);//将便道的车进入车库

int n, cost, j = 0, k = 0;//n是用户输入停车场停放的车，cost是费用
int main() {
	int car, time_hour, time_minute = 1;//time停放时间
	char ch = ' ';//车的状态
	char option[10];//查询密令
	StopCar sc;//停车场
	InitStopCar(&sc);
	WaitCarLink wcl;
	InitWaitCarLink(&wcl);
	WaitCar* wc;//创建结点指针

	printf("请输入停车场可以停放的最大车辆以及停车每小时收取的费用（元）（用空格间隔）：");
	scanf("%d %d", &n, &cost);
	while (1)
	{
		fflush(stdin);//清除缓存区，免得多循环一次
		printf("\n请输入车的状态(A到达，D离去, C查询），车的车牌号，车的时间（到达时间，离去时间）：");
		scanf(" %c %d %d:%d", &ch, &car, &time_hour, &time_minute);//这里缓冲区残留的换行符导致报错过，解法是%c前加空格
		//printf("ch=%c, car=%d, time=%d\n", ch, car, time);输出一下，验证缓冲区问题是否解决
		/*if(car<10000||car>99999)
        {
            printf("车牌号非法");
            break;
        }*/
		if(time_hour>24||time_hour<0||time_minute>60||time_minute<0)
        {
            printf("时间有误，请检查时间问题");
            break;
        }
		if (ch=='A')
		{
			ArrivalCar(&sc, &wcl, ch, car, time_hour, time_minute);
		}
		else if (ch=='D')
		{
			DepartureCar(&sc, ch, car, time_hour, time_minute);
			WaitToStop(&sc, &wcl);//直接将便道上的车移到车库
		}
		else if (ch=='C')
        {
            printf("是否进入查询界面(是/否)？");
            scanf("%s", option);
            if (strcmp(option, "是") == 0)
            {
                printf("进入查询界面中……\n");
                PrintAllCars(sc);
            }
        }
		else
		{
			printf("请输入正确的编号！\n");
		}
	}
	return 0;
}
void WaitToStop(StopCar* sc, WaitCarLink* wcl)//将便道上的车移到车库
{
	if (wcl->front == wcl->rear)//便道没有车
		printf("没有便道车！\n");
	else {
		//出队
		WaitCar *S;
		S = wcl->front->next;//S指向队头
		wcl->front->next = S->next;//S指向的队头出队
		if (wcl->rear == S)//如果S是最后一个元素，让队尾指针指向队头防止变成野指针
			wcl->rear = wcl->front;
		printf("%d出道成功！\n", S->data[k].car);
		//进栈
		sc->data[sc->top + 1].car = S->data[k].car;
		sc->data[sc->top + 1].ch = S->data[k].ch;
		sc->data[sc->top + 1].time_hour = S->data[k].time_hour;
		sc->data[sc->top + 1].time_minute = S->data[k].time_minute;
		printf("%d进库成功！\n", S->data[k].car);
		k++;
		sc->top++;
	}
}
void DepartureCar(StopCar* sc, char ch, int car, int time_hour, int time_minute)//离开
{
	TemporaryCar tc;
	InitTemporaryCar(&tc);
	int timesum;
	if (sc->top == -1)
		printf("车库没有车！无法离开\n");
	else if (IfTheCar(sc,car)) {
		printf("车库里没有此车！\n");
	}
	else
	{
		while (sc->data[sc->top].car != car)//在此车前面的车进入便道
		{
			tc.data[tc.top + 1].car = sc->data[sc->top].car;
			tc.data[tc.top + 1].ch = sc->data[sc->top].ch;
			tc.data[tc.top + 1].time_hour = sc->data[sc->top].time_hour;
			tc.data[tc.top + 1].time_minute = sc->data[sc->top].time_minute;
			tc.top++;
			sc->top--;//此车出栈
		}
		if (time_hour < sc->data[sc->top].time_hour)
        {
            printf("停车超过12小时，请输入停车天数（不满一天按一天算）：");
            int u = 0;
            scanf("%d",&u);
            time_hour+=24;
        }
		timesum = (time_hour - sc->data[sc->top].time_hour);
		if (time_minute - sc->data[sc->top].time_minute > 0)
        {
            timesum++;
        }
		printf("车牌是%d在停车停留的时间是%d小时,停车的费用是%d元\n", sc->data[sc->top].car, timesum,timesum * cost);
		sc->top--;
		while (tc.top != -1)//临时停车区进入停车场
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
        printf("位置 %d 的车牌号是：%d\n", i + 1, sc->data[i].car);
    }
}
void ArrivalCar(StopCar* sc, WaitCarLink* wcl, char ch, int car, int time_hour, int time_minute)//入库函数
{

	WaitCar* wc;//创建结点指针
	wc = (WaitCar*)malloc(sizeof(WaitCar));
	wc->next = NULL;
	if ((sc->top + 1) < n) {//车库没满
		sc->data[sc->top + 1].ch = ch;
		sc->data[sc->top + 1].car = car;
		sc->data[sc->top + 1].time_hour = time_hour;
		sc->data[sc->top + 1].time_minute = time_minute;
		sc->top++;
		printf("\n车牌号为%d的车停的位置是：%d\n", car, sc->top + 1);
	}
	else//满了进便道
	{
		wc->data[j].car = car;
		wc->data[j].ch = ch;
		wc->data[j].time_hour = time_hour;
		wc->data[j].time_minute = time_minute;
		wc->next = NULL;
		wcl->rear->next = wc;
		wcl->rear = wc;
		printf("车牌号为%d的车在便道上的位置是：%d\n", car, j + 1);
		j++;
	}
}
int IfTheCar(StopCar *sc, int car)//判断是否有输入这辆车
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
void InitStopCar(StopCar* sc)//停车场初始化
{
	sc->top = -1;
}
int InitWaitCarLink(WaitCarLink* wcl)//等候区初始化
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
void InitTemporaryCar(TemporaryCar* tc)//临时停车初始化
{
	tc->top = -1;
}

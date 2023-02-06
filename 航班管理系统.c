#include<stdio.h>
#include<string.h>
#include<malloc.h>
#define MaxSpace 100
#define keylen 6
#define RADIX_n 10
#define RADIX_c 26
typedef char KeyType;
typedef struct{
    char start[8];                    //���
    char end[8];                     //�յ�
    char sche[8];                    //���� 
    char time1[6];                  //���ʱ�� 
    char time2[6];                  //����ʱ�� 
    char model[6];                 //���� 
    int price;                         //Ʊ�� 
}InfoType;                      //�����¼���� 
typedef struct{
    KeyType keys[keylen];          //�ؼ��� 
    InfoType others;        
    int next;
}SLNode;                                //��ڵ� 
typedef struct{
    SLNode s1[MaxSpace];           //��̬������s1[0]Ϊͷ���
    int keynum;                             //�ؼ��ֳ�
    int length;                                //��ǰ��
}SLList;                                   //��̬��������

typedef int ArrType_n[RADIX_n];
typedef int ArrType_c[RADIX_c];
int m=0,num=0;

/*���㷨ʱ���ؼ���keys[i]����radix���ӱ�ʹͬһ���ӱ��м�¼��keys[i]��ͬ��f[0��radix]

  ��e[0..radix]�ֱ�ָ����ӱ�ʹͬһ���ӱ��еĵ�һ�������һ����¼*/
void Distribute(SLNode *s1,int i,ArrType_n f,ArrType_n e)
{
    int j,p;
    for(j=0;j<RADIX_n;j++)                    /*���ӱ��ʼ��*/
    {
        f[j]=0;
        e[j]=0;
    }
    for(p=s1[0].next;p;p=s1[p].next)
    {
        j=s1[p].keys[i]%48;                  /*�������ַ�ת���ɶ�Ӧ����ֵ������*/
        if(!f[j])   f[j]=p;
        else s1[e[j]].next=p;
        e[j]=p;                              /*��pָ��Ľ����뵽��j�����*/
    }
}

/*���㷨�ǰ��ؼ���keys[i]��С����[0��radix]��ָ�ĸ��ӱ��������ӳ�һ������*/
void Collect(SLNode *s1,ArrType_n f,ArrType_n e)
{
    int j,t;
    for(j=0;!f[j];j++);                      /*�ҵ���һ���ǿ��ӱ�*/
    s1[0].next=f[j];                         /*sl[0].nextָ���һ���ǿ��ӱ��е�һ�����*/
    t=e[j];
    while(j<RADIX_n-1)
    {
        for(j=j+1;j<RADIX_n-1 && !f[j];j++); /*����һ���ǿ��ӱ�*/
        if(f[j])                             /*���������ǿ��ֱ�*/
        {
            s1[t].next=f[j];
            t=e[j];
        }
    }
    s1[t].next=0;                           /*tָ�����һ���ǿ��ӱ�*/
}

void Distribute_c(SLNode *s1,int i,ArrType_c f,ArrType_c e)
{
    int j,p;
    for(j=0;j<RADIX_c;j++)
    {

        f[j]=0;
        e[j]=0;
    }
    for(p=s1[0].next;p!=0;p=s1[p].next)
    {
        j=s1[p].keys[i]%65;
        if(!f[j])  f[j]=p;
        else s1[e[j]].next=p;
        e[j]=p;
    }
}

void Collect_c(SLNode *s1,ArrType_c f,ArrType_c e)
{
    int j,t;
    for(j=0;!f[j];j++);
    s1[0].next=f[j];
    t=e[j];
    while(j<RADIX_c-1)
    {
        for(j=j+1;j<RADIX_c-1 && !f[j];j++);
        if(f[j])
        {
            s1[t].next=f[j];
            t=e[j];
        }
    }
    s1[t].next=0;
}

/*���㷨�ǰ��ؼ��ִӵ�λ����λ���ζԸ��ؼ��ֽ��з�����ռ���������ʵ�֣�����RadixSort����ͨ������ǰ��ķ��亯�����ռ������������һ���ؼ��ֿ�ʼ��������*/


void RadixSort(SLList *L)
{
    int i;
    ArrType_n fn,en;
    ArrType_c fc,ec;
    for(i=0;i<L->length;i++)
        L->s1[i].next=i+1;
    L->s1[L->length].next=0;                  /*������Ϊ��̬����*/
    for(i=L->keynum-1;i>=2;i--)            /*�����λ�������ζԸ��ؼ��ֽ��з�����ռ�*/
    {
        Distribute(L->s1,i,fn,en);
        Collect(L->s1,fn,en);
    }
    for(i=1;i>=0;i--)
    {
        Distribute_c(L->s1,i,fc,ec);
        Collect_c(L->s1,fc,ec);
    }
}

void Arrange(SLList *L)
{
    int p,q,i;
    SLNode temp;
    p=L->s1[0].next;                       /*pָʾ��һ����¼�ĵ�ǰλ��*/
    for(i=1;i<L->length;i++)
    {
        while(p<i)                                /*�ҵ���i����¼������pָʾ���ڵ�λ��*/
            p=L->s1[p].next;
        q=L->s1[p].next;                      /*qָʾ��δ�����ı�β*/
        if(p!=i)
        {
            temp=L->s1[p];
            L->s1[p]=L->s1[i];
            L->s1[i]=temp;
            L->s1[i].next=p;                     /*ָ�����ߵļ�¼*/
        }
        p=q;                               /*pָ����δ�����ı�β��Ϊ��һ��׼��*/
    }
}

/*LΪ�����ҵı�key[]Ϊ�����ҵĹؼ��֣������ֲ��ҵ�˼��ʵ�ֲ���*/
int BinSearch(SLList *L,KeyType key[])
{
    int low,high,mid;
    low=1;
    high=L->length;
    while(low<=high)
    {
        mid=(low+high)/2;
        if(strcmp(key,L->s1[mid].keys)==0)  return mid;
        else if(strcmp(key,L->s1[mid].keys)<0)   high=mid-1;
        else low=mid+1;
    }
    return 0;
}

void Display(SLList *L,int i)
{
    printf("�����  ���վ   �յ�վ    ������   ���ʱ��  ����ʱ��  ����    Ʊ��\n");
    printf("%6s,%6s,%8s,%8s,%8s,%8s,%8s,%6d\n",L->s1[i].keys,L->s1[i].others.start,L->s1[i].others.end,L->s1[i].others.sche,L->s1[i].others.time1,L->s1[i].others.time2,L->s1[i].others.model,L->s1[i].others.price);
}


void SeqSearch(SLList *L,KeyType key[],int i)
{
    int j,k,m=0;
    for(j=1;j<=L->length;j++)
    {
        switch(i)
        {
            case 2:k=strcmp(key,L->s1[j].others.start);
                   break;
            case 3:k=strcmp(key,L->s1[j].others.end);
                   break;
            case 4:k=strcmp(key,L->s1[j].others.time1);
                   break;
            case 5:k=strcmp(key,L->s1[j].others.time2);
                   break;
        }
        if(k==0)
        {
            m=1;
            Display(L,j);
        }
    }
    if(m==0)  printf("�޴˺�����Ϣ���������������!\n");
}

/*���ڴ��㷨��Ҫ��ע����Ϣ���ȵĸ�ֵ������ÿ�ε���Ҫ����Ӧ�ĸı䡣*/
void InputData(SLList *L)
{
    int i,j,n;
    char kn;
    char yn='y';
    num++;
    if(num==1)
    {
        i=++L->length;
    }
    else
    {
        i=L->length;
    } 
    while(yn=='y' || yn=='Y')
    {
        m++;
        n=0;
        printf("�����  ���վ   �յ�վ  ������   ���ʱ��  ����ʱ��  ����  Ʊ��");
        puts("");
        scanf("%s %s %s %s %s %s %s %d",L->s1[i].keys,L->s1[i].others.start,L->s1[i].others.end,L->s1[i].others.sche,L->s1[i].others.time1,L->s1[i].others.time2,L->s1[i].others.model,&L->s1[i].others.price);
        scanf("%c",&kn);
        if(m!=1)
        {
            for(j=1;j<i;j++)
            {
                if(strcmp(L->s1[i].keys,L->s1[j].keys)==0)
                {
                    printf("sorry,������ĺ����Ѿ�����!\n");
                    n=1;
                    break;
                }
            }
        }

        if(n==0) i++;
        printf("����������y/n:"); 
        scanf("%c", &yn);
    }
    L->length=i;
    if(n==0)
    {
        RadixSort(L);
        Arrange(L);
    }
}


void serachcon(SLList *L,int n)
{
    int i=1;
    while(i!=0)
    {
        char key[keylen];
        char kl[4];
        int k,j;
        printf("/t*************************************\n");
		printf("/t**********������Ϣ��ѯϵͳ***********\n");
    	printf("/t*************************************\n");
  	 	printf("/t*******   1. �� �� ��         *******\n");
  		printf("/t*******   2. �� �� վ         *******\n");
  		printf("/t*******   3. �� �� վ         *******\n");
  		printf("/t*******   4. ���ʱ��         *******\n");
  		printf("/t*******   5. ����ʱ��         *******\n");
  		printf("/t*******   6. ��ʾ���к�����Ϣ *******\n");
        printf("/t*******   0. �˳�ϵͳ         *******\n");
        printf("/t*************************************\n");
        printf("/t*******   ��ѡ��0��6��      *******\n");
        scanf("%d",&i);
        if(i==1)
        {
            printf("����Ҫ��ѯ�ĺ����(���ִ�Сд):");
            scanf("%s",key);
            k=BinSearch(L,key);
            if(k==0)
                printf("�޴˺�����Ϣ���������������!\n");
            else Display(L,k);
        }
        else if(i==2)
        {
            printf("����Ҫ��ѯ�ĺ������վ��:");
            scanf("%s",key);
            SeqSearch(L,key,i);
        }
        else if(i==3)
        {
            printf("����Ҫ��ѯ�ĺ����յ�վ��:");
            scanf("%s",key);
            SeqSearch(L,key,i);
        }
        else if(i==4)
        {        
			printf("����Ҫ��ѯ�ĺ������ʱ��:");
            scanf("%s",kl);
            SeqSearch(L,kl,i);
        }
        else if(i==5)
        {
            printf("����Ҫ��ѯ�ĺ��ൽ��ʱ��:");
            scanf("%s",kl);
            SeqSearch(L,kl,i);
        }
        else if(i==6)
        {
            if(L->length==1)
                printf("�Բ���û�к�����Ϣ!\n");
            else
            {
                printf("��ʾ���к�����Ϣ:\n");
                for(j=1;j<L->length;j++)
                {
                    Display(L,j+1);
                }
            }
        }
        else if(i==0)
        {
            printf("�ټ�!\n");
            return ;
        }
        else printf("�޴˲��������������������������!\n");
    }
}

void login(SLList *L)
{
    char str[10];
    int i=1;
    printf("if you are administrator,please Login! or input  to exit !\n");
    while(i){
        printf("Input you username:");
        scanf("%s",&str);
        if(strcmp(str,"admin")==0)
        {
            printf("\nInput you password:");
            scanf("%s",&str);
            if(strcmp(str,"123456")==0)
            {
                serachcon(L,i);
                break;
            }
            else
                printf("you password is wrong!\n");
        }
        else if(strcmp(str,"exit")==0)
        {
            i=0;
            serachcon(L,i);
        }
        else
            printf("you username is wrong!\n");
    }
}

int main()
{
    SLList *L;
    L=(SLList *)malloc(sizeof(SLList));
    L->keynum=6;
    L->length=0;
    InputData(L);
    login(L);
}

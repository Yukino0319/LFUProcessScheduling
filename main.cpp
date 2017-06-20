#include <stdio.h>

#define PROCESS_SHCEDULING_MAX 12
#define PAGE_TABLE_MAX 4

int PageTableIsNULL(int aPage_Table[][PROCESS_SHCEDULING_MAX], int anI);    //ҳ���Ƿ��п���ҳ
int PageTableIsHit(int aPage_Table[][PROCESS_SHCEDULING_MAX], int anI, int aProcessNum);    //ҳ���Ƿ�����
int WhereIsHit(int aPage_Table[][PROCESS_SHCEDULING_MAX], int anI, int aProcessNum);    //ҳ������λ��
int WhereIsNULL(int aPage_Count[], int aStatus);          //��ѯ��ǰҳ���еĿ�ҳ��������ʹ��ҳ����λ��λ��
void OutPutProcessScheduling(int aPage_Table[][PROCESS_SHCEDULING_MAX], int aProcess_Scheduling[], char aPage_Table_Flag[], int aPage_Table_Hit_Count);

int main(){
    //���̵�������
    int process_scheduling[PROCESS_SHCEDULING_MAX] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
    //ҳ��
    int page_table[PAGE_TABLE_MAX][PROCESS_SHCEDULING_MAX];
    //ҳ�������
    int page_count[PAGE_TABLE_MAX];
    //ҳ���ʼΪ��
    for(int i = 0; i < PAGE_TABLE_MAX; ++i)
        page_table[i][0] = 0;
    //ҳ���������0
    for(int i = 0; i < PAGE_TABLE_MAX; ++i)
        page_count[i] = 0;
    //��̭˳��
    int off_order[PROCESS_SHCEDULING_MAX];
    int buf_offorder = 0;
    //ҳ���Ƿ����У�'Y'��ʾȱҳ��'N'��ʾ����
    char page_table_flag[PROCESS_SHCEDULING_MAX];
    int page_table_hit_count = 0;
    int location = 0;

    printf("���̵���ʱҳ��������ı仯��\n");
    printf("========================================\n");
    printf("i\tp[0]\tp[1]\t...\n\n");
    for(int i = 0; i < PROCESS_SHCEDULING_MAX; i++){
        //���̵���
        //������У�����ҳ����Ϣ
        if(!PageTableIsHit(page_table, i, process_scheduling[i])){
            location = WhereIsHit(page_table, i, process_scheduling[i]);
            if(location){
                int temp = page_count[location];
                for(int j = location; j >= 1; j--){
                    page_table[j][i] = page_table[j-1][i];\
                    page_count[j] = page_count[j-1];
                }
                page_count[0] = temp;
                page_table[0][i] = process_scheduling[i];
            }
            page_count[0]++;
            page_table_flag[i] = 'N';
            page_table_hit_count++;
        }
        //δ���У�����ȱҳ�ж�
        else{
            //��ҳ��Ϊ�գ�˵����ҳ�汻��̭������̭��ҳ�����ҳ����̭˳����У������Ӧ�ļ�������0��
            //�²����ҳ����뵽��0�У�С��location��ҳ��������1��
            /**
             *�ٴ�������ҳ�������page_count[]��ʾҳ��ʹ��Ƶ�ʣ��������ж����ͼ���ʱ��
             *Ӧ����ҳ�����ҳ���˳����̭�����ʱ��˳����page_table[row][col]��row�洢��
             *����ҳ���ĳҳ�汻����ʱ������������
             */
            location = WhereIsNULL(page_count, PageTableIsNULL(page_table, i));
            if(PageTableIsNULL(page_table, i)){
                off_order[buf_offorder++] = page_table[location][i];
                page_count[location] = 0;
            }
            for(int j = location; j >= 1; j--){
                page_table[j][i] = page_table[j-1][i];
                page_count[j] = page_count[j-1];
            }
            page_table[0][i] = process_scheduling[i];
            page_count[0] = 1;
            page_table_flag[i] = 'Y';
        }
        if(i < PROCESS_SHCEDULING_MAX - 1){
            for (int j = 0; j < PAGE_TABLE_MAX; ++j)
                page_table[j][i+1] = page_table[j][i];
        }
        printf("%d\t", i);
        for(int j = 0; j < PAGE_TABLE_MAX; j++)
            printf("%d\t", page_count[j]);
        printf("\n");
    }

    OutPutProcessScheduling(page_table, process_scheduling, page_table_flag, page_table_hit_count);

    //�����̭˳��
    printf("��̭˳��");
    for(int k = 0; k < buf_offorder; k++)
        printf("%d ", off_order[k]);
    printf("\n");
    return 0;
}

/**
 * �ж�ҳ���Ƿ��п���ҳ
 * @param aPage_Table[][PROCESS_SHCEDULING_MAX]
 *          ҳ��
 * @param anI
 *          ��ǰ���̵��ȴ���
 * @return
 *          ҳ���п���ҳ��0���޿���ҳ��1
 */
int PageTableIsNULL(int aPage_Table[][PROCESS_SHCEDULING_MAX], int anI){
    //����ǵ�һ�ν��̵���
    if(0 == anI) return 0;
    //���Ϊ�շ���0�����򷵻�1
    return (0 == aPage_Table[PAGE_TABLE_MAX-1][anI - 1] ? 0 : 1);
}

/**
 * �жϵ�ǰ���̵����Ƿ�����ҳ��
 * @param aPage_Table[][PROCESS_SHCEDULING_MAX]
 *          ҳ��
 * @param anI
 *          ��ǰ���̵��ȴ���
 * @param aProcessNum
 *          ��ǰ���̵��Ⱥ�
 * @return
 *          ҳ�����У�0��δ���У�1
 */
int PageTableIsHit(int aPage_Table[][PROCESS_SHCEDULING_MAX], int anI, int aProcessNum){
    //����ǵ�һ�ν��̵��ȣ���ȱҳ
    if(0 == anI) return 1;
    //������з���0�����򷵻�1
    for(int i = 0; i < PAGE_TABLE_MAX; i++){
        if(aProcessNum == aPage_Table[i][anI-1]){
            return 0;
        }
        else{}
    }
    return 1;
}

/**
 * ��ѯ��ǰ���̵�������ҳ��ʱ��ҳ����λ��
 * @param aPage_Table[][PROCESS_SHCEDULING_MAX]
 *          ҳ��
 * @param anI
 *          ��ǰ���̵��ȴ���
 * @param aProcessNum
 *          ��ǰ���̵��Ⱥ�
 * @return i
 *          ҳ������
 */
int WhereIsHit(int aPage_Table[][PROCESS_SHCEDULING_MAX], int anI, int aProcessNum){
    for(int i = 0; i < PAGE_TABLE_MAX; i++){
        if(aProcessNum == aPage_Table[i][anI-1]){
            return i;
        }
    }
}

/**
 * ��ѯ��ǰҳ���еĿ�ҳ��������ʹ��ҳ����λ��λ��
 * @param aPage_Count[]
 *          ҳ�������
 * @param aStatus
 *          ҳ��״̬��0��ʾҳ���п��У�1��ʾҳ��Ϊ��
 * @return i/row
 *          �ɲ�����滻���к�
 */
int WhereIsNULL(int aPage_Count[], int aStatus){
    //���ҳ���п�����
    if(!aStatus){
        for(int i = 0; i < PAGE_TABLE_MAX; ++i){
            if (!aPage_Count[i])
                return i;
        }
    }
    //����޿�����ҳ�������С��һ����ж�������������ֵ
    //������������ֵ��ʾ����ҳ�����
    //ÿ��ҳ�������滻��ҳ�����ҳ���������
    else{
        int min_count = aPage_Count[0];
        int row = 0;
        for(int i = 1; i < PAGE_TABLE_MAX; ++i){
            row = aPage_Count[i] <= min_count ? i : row;
            min_count = aPage_Count[i] <= min_count ? aPage_Count[i] : min_count;
        }
        return row;
    }
}
/**
 * �жϵ�ǰ���̵����Ƿ�����ҳ��
 * @param aPage_Table[][PROCESS_SHCEDULING_MAX]
 *          ҳ��
 * @param aProcess_Scheduling[]
 *          ���̵�������
 * @param aPage_Table_Flag[]
 *          ĳ�ν��̵���ʱ��ҳ���Ƿ�ȱҳ����
 * @param aPage_Table_Hit_Count
 *          ҳ�����д���
 * @return
 */
void OutPutProcessScheduling(int aPage_Table[][PROCESS_SHCEDULING_MAX], int aProcess_Scheduling[], char aPage_Table_Flag[], int aPage_Table_Hit_Count){
    printf("���̵������м�ҳ������㷨��N��ʾδ���У�Y��ʾ���У���\n");
    printf("========================================\n");
    printf(" ");
    for(int i = 0; i < PROCESS_SHCEDULING_MAX; i++)
        printf("  %d", aProcess_Scheduling[i]);
    printf("\n");
    printf("\n");
    for(int i = 0; i < PAGE_TABLE_MAX; i++){
        printf("0");
        for(int j = 0; j < PROCESS_SHCEDULING_MAX; j++)
            printf("  %d", aPage_Table[i][j]);
        printf("\n");
    }
    printf(" ");
    for(int i = 0; i < PROCESS_SHCEDULING_MAX; i++)
        printf("  %c", aPage_Table_Flag[i]);
    printf("\n");
    printf("ȱҳ�� = %.2f%%\n", (PROCESS_SHCEDULING_MAX - aPage_Table_Hit_Count) * 100.0 / PROCESS_SHCEDULING_MAX);
    printf("========================================\n");
}

#include <stdio.h>

#define PROCESS_SHCEDULING_MAX 12
#define PAGE_TABLE_MAX 4
#define PAGE_COUNT_ALLZERO_T 4

int PageTableIsNULL(int aPage_Table[][PROCESS_SHCEDULING_MAX], int anI);    //页表是否有空闲页
int PageTableIsHit(int aPage_Table[][PROCESS_SHCEDULING_MAX], int anI, int aProcessNum);    //页表是否命中
int WhereIsHit(int aPage_Table[][PROCESS_SHCEDULING_MAX], int anI, int aProcessNum);    //页表命中位置
int WhereIsNULL(int aPage_Count[], int aStatus);          //查询当前页表中的空页表项及最不经常使用页表项位置位置
void OutPutProcessScheduling(int aPage_Table[][PROCESS_SHCEDULING_MAX], int aProcess_Scheduling[], char aPage_Table_Flag[], int aPage_Table_Hit_Count);

int main(){
    //进程调度序列
    int process_scheduling[PROCESS_SHCEDULING_MAX] = {4, 3, 2, 1, 4, 3, 5, 4, 3, 2, 1, 5};
    //页表
    int page_table[PAGE_TABLE_MAX][PROCESS_SHCEDULING_MAX];
    //页面计数器
    int page_count[PAGE_TABLE_MAX];
    //页表初始为空
    for(int i = 0; i < PAGE_TABLE_MAX; ++i)
        page_table[i][0] = 0;
    //页面计数器清0
    for(int i = 0; i < PAGE_TABLE_MAX; ++i)
        page_count[i] = 0;
    //淘汰顺序
    int off_order[PROCESS_SHCEDULING_MAX];
    int buf_offorder = 0;
    //页表是否被命中，'Y'表示缺页，'N'表示命中
    char page_table_flag[PROCESS_SHCEDULING_MAX];
    int page_table_hit_count = 0;
    int location = 0;
    //未发生缺页中断次数
    int noint_count = 0;
    printf("进程调度时页面计数器的变化：\n");
    printf("========================================\n");
    printf("i\tp[0]\tp[1]\t...\n\n");
    for(int i = 0; i < PROCESS_SHCEDULING_MAX; i++){
        if(noint_count && 0 == (noint_count % PAGE_COUNT_ALLZERO_T)){
	    for(int j = 0; j < PAGE_TABLE_MAX; j++)
		page_count[j] = 0;
	}
        //如果命中，更新页表信息
	if(!PageTableIsHit(page_table, i, process_scheduling[i])){
	    noint_count++;
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
        //未命中，发生缺页中断
        else{
            //若页表不为空，说明有页面被淘汰，将淘汰的页面存入页面淘汰顺序表中，将其对应的计数器清0。
            //新插入的页面插入到第0行，小于location的页面行数加1。
            /**
             *再次声明，页面计数器page_count[]表示页面使用频率，当遇到有多个最低计数时，
             *应按照页面进入页表的顺序淘汰，这个时间顺序由page_table[row][col]的row存储。
             *当新页面或某页面被命中时，调整行数。
             */
	    noint_count = 0;
            location = WhereIsNULL(page_count, PageTableIsNULL(page_table, i));
            if(PageTableIsNULL(page_table, i)){
                off_order[buf_offorder++] = page_table[location][i];
		for(int k = 0; k < PAGE_TABLE_MAX; k++)
		    page_count[k] = 0;
                //page_count[location] = 0;
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

    //输出淘汰顺序
    printf("淘汰顺序:");
    for(int k = 0; k < buf_offorder; k++)
        printf("%d ", off_order[k]);
    printf("\n");
    return 0;
}

/**
 * 判断页表是否有空闲页
 * @param aPage_Table[][PROCESS_SHCEDULING_MAX]
 *          页表
 * @param anI
 *          当前进程调度次数
 * @return
 *          页表有空闲页：0；无空闲页：1
 */
int PageTableIsNULL(int aPage_Table[][PROCESS_SHCEDULING_MAX], int anI){
    //如果是第一次进程调度
    if(0 == anI) return 0;
    //如果为空返回0，否则返回1
    return (0 == aPage_Table[PAGE_TABLE_MAX-1][anI - 1] ? 0 : 1);
}

/**
 * 判断当前进程调度是否命中页表
 * @param aPage_Table[][PROCESS_SHCEDULING_MAX]
 *          页表
 * @param anI
 *          当前进程调度次数
 * @param aProcessNum
 *          当前进程调度号
 * @return
 *          页表命中：0；未命中：1
 */
int PageTableIsHit(int aPage_Table[][PROCESS_SHCEDULING_MAX], int anI, int aProcessNum){
    //如果是第一次进程调度，必缺页
    if(0 == anI) return 1;
    //如果命中返回0，否则返回1
    for(int i = 0; i < PAGE_TABLE_MAX; i++){
        if(aProcessNum == aPage_Table[i][anI-1]){
            return 0;
        }
        else{}
    }
    return 1;
}

/**
 * 查询当前进程调度命中页表时的页表项位置
 * @param aPage_Table[][PROCESS_SHCEDULING_MAX]
 *          页表
 * @param anI
 *          当前进程调度次数
 * @param aProcessNum
 *          当前进程调度号
 * @return i
 *          页表行数
 */
int WhereIsHit(int aPage_Table[][PROCESS_SHCEDULING_MAX], int anI, int aProcessNum){
    for(int i = 0; i < PAGE_TABLE_MAX; i++){
        if(aProcessNum == aPage_Table[i][anI-1]){
            return i;
        }
    }
}

/**
 * 查询当前页表中的空页表项及最不经常使用页表项位置位置
 * @param aPage_Count[]
 *          页面计数器
 * @param aStatus
 *          页表状态，0表示页表有空闲，1表示页表不为空
 * @return i/row
 *          可插入或替换的行号
 */
int WhereIsNULL(int aPage_Count[], int aStatus){
    //如果页表有空闲项
    if(!aStatus){
        for(int i = 0; i < PAGE_TABLE_MAX; ++i){
            if (!aPage_Count[i])
                return i;
        }
    }
    //如果无空闲项，找出计数最小的一项，若有多项，返回行数最大值
    //这里的行数最大值表示进入页表较早
    //每次页面插入或替换入页表都会对页表进行排序
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
 * 判断当前进程调度是否命中页表
 * @param aPage_Table[][PROCESS_SHCEDULING_MAX]
 *          页表
 * @param aProcess_Scheduling[]
 *          进程调度序列
 * @param aPage_Table_Flag[]
 *          某次进程调度时，页表是否缺页序列
 * @param aPage_Table_Hit_Count
 *          页表命中次数
 * @return
 */
void OutPutProcessScheduling(int aPage_Table[][PROCESS_SHCEDULING_MAX], int aProcess_Scheduling[], char aPage_Table_Flag[], int aPage_Table_Hit_Count){
    printf("进程调度序列及页面调度算法（N表示未命中，Y表示命中）：\n");
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
    printf("缺页率 = %.2f%%\n", (PROCESS_SHCEDULING_MAX - aPage_Table_Hit_Count) * 100.0 / PROCESS_SHCEDULING_MAX);
    printf("========================================\n");
}

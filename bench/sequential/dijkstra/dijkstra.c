// LICENSE: GPL. This file comes from the network section of the MiBench

#include "glibc_common.h"
#include "input.h"

#define NONE 9999
#define OUT_OF_MEMORY -1


struct _NODE
{
  int iDist;
  int iPrev;
};

struct _QITEM
{
  int iNode;
  int iDist;
  int iPrev;
  struct _QITEM *qNext;
};

struct _QITEM *qHead;

int g_qCount = 0;
struct _NODE rgnNodes[NUM_NODES];
int ch;
int iPrev, iNode;
int i, iCost, iDist;

#define QUEUE_SIZE 1000

struct _QITEM qitems[QUEUE_SIZE];
int next_in = 0;

int enqueue(int iNode, int iDist, int iPrev)
{
  struct _QITEM *qNew = &qitems[next_in];

  struct _QITEM *qLast = qHead;

  if (++next_in >= QUEUE_SIZE) {
    return OUT_OF_MEMORY;
  }
  qNew->iNode = iNode;
  qNew->iDist = iDist;
  qNew->iPrev = iPrev;
  qNew->qNext = (void*)NULL;

  if (!qLast) {
    qHead = qNew;
  }
  else {
    _Pragma( "loopbound min 0 max 313" )
    while (qLast->qNext)
      qLast = qLast->qNext;
    qLast->qNext = qNew;
  }
  g_qCount++;
  return 0;
}

void dequeue(int *piNode, int *piDist, int *piPrev)
{
  //struct _QITEM *qKill = qHead;

  if (qHead) {
    *piNode = qHead->iNode;
    *piDist = qHead->iDist;
    *piPrev = qHead->iPrev;
    qHead = qHead->qNext;
    //    my_free(qKill);
    g_qCount--;
  }
}

int qcount(void)
{
  return(g_qCount);
}

int dijkstra(int chStart, int chEnd)
{
  _Pragma( "loopbound min 100 max 100" )
  for (ch = 0; ch < NUM_NODES; ch++) {
    rgnNodes[ch].iDist = NONE;
    rgnNodes[ch].iPrev = NONE;
  }

  if (chStart == chEnd) {
  }
  else {
    rgnNodes[chStart].iDist = 0;
    rgnNodes[chStart].iPrev = NONE;

    if (enqueue (chStart, 0, NONE) == OUT_OF_MEMORY)
      return OUT_OF_MEMORY;

    _Pragma( "loopbound min 618 max 928" )
    while (qcount() > 0) {
      dequeue (&iNode, &iDist, &iPrev);
      _Pragma( "loopbound min 100 max 100" )
      for (i = 0; i < NUM_NODES; i++) {
        if ((iCost = AdjMatrix[iNode][i]) != NONE) {
          if ((NONE == rgnNodes[i].iDist) ||
                  (rgnNodes[i].iDist > (iCost + iDist))) {
            rgnNodes[i].iDist = iDist + iCost;
            rgnNodes[i].iPrev = iNode;
            if (enqueue (i, iDist + iCost, iNode) == OUT_OF_MEMORY)
              return OUT_OF_MEMORY;
          }
        }
      }
    }
  }
  return 0;
}

int main()
{
  int i,j;

  qHead = (void*)NULL;
  /* finds 10 shortest paths between nodes */
  _Pragma( "loopbound min 20 max 20" )
  for (i=0,j=NUM_NODES/2;i<20;i++,j++) {
    j=j%NUM_NODES;
    if (dijkstra(i,j) == OUT_OF_MEMORY)
      return OUT_OF_MEMORY;
    next_in = 0;
  }
  
  return 0;
}


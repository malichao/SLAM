#include <cstdio>
#include <cstring>
#include <queue>
using namespace std;
const int MAXN = 5;

int m = MAXN, n = MAXN;

int map[MAXN][MAXN]={
      0,1,0,0,0,
      0,1,0,1,0,
      0,0,0,0,0,
      0,1,1,1,0,
      0,0,0,1,0
};

const char name[4] = { 'U', 'R', 'D', 'L' };
const int dx[4] = { -1, 0, 1, 0 };
const int dy[4] = { 0, 1, 0, -1 };

typedef struct state_t {
   int data;
   int action;
   int father;
} state_t;
const int STATE_MAX = MAXN * MAXN;

state_t nodes[STATE_MAX];
int state_hash(const state_t &s);
int state_index(const state_t &s) {
   return state_hash(s);
}
void print_action(const int end) {
   if (nodes[end].father == -1)
      return;
   print_action(nodes[end].father);
   putchar(name[nodes[end].action]);
}
void print_path(const int end) {
   if (nodes[end].father == -1) {
      printf("(%d, %d)\n", end / n, end % n);
      return;
   }
   print_path(nodes[end].father);
   printf("(%d, %d)\n", end / n, end % n);
}
void hashset_init();
bool hashset_find(const state_t &s);
void hashset_insert(const state_t &s);
void state_extend_init(const state_t &s);
bool state_extend(const state_t &s, state_t &next);
bool state_is_target(const state_t &s);
int bfs(state_t &start) {
   queue<state_t> q;
   hashset_init();
   start.action = -1;
   start.father = -1;
   nodes[state_index(start)] = start;
   hashset_insert(start);
   if (state_is_target(start))
      return state_index(start);
   q.push(start);
   while (!q.empty()) {
      const state_t s = q.front();
      q.pop();
      state_t next;

      state_extend_init(s);
      while (state_extend(s, next)) {
         if (state_is_target(next)) {
            return state_index(next);
         }
         q.push(next);
         hashset_insert(next);
      }
   }
   return -1;
}
int main(void) {
   state_t start = { 0, -1, -1 };
   int end;
   end = bfs(start);
   print_path(end);
   return 0;
}
/********** functions implement **************/

const int HASH_CAPACITY = STATE_MAX;
bool visited[HASH_CAPACITY];
int state_hash(const state_t &s) {
   return s.data;
}
void hashset_init() {
   memset(visited, 0, sizeof(visited));
}
bool hashset_find(const state_t &s) {
   return visited[state_hash(s)] == true;
}
void hashset_insert(const state_t &s) {
   visited[state_hash(s)] = true;
}
int action_cur;
#define ACTION_BEGIN 0
#define ACTION_END 4

int x, y;
void state_extend_init(const state_t &s) {
   action_cur = ACTION_BEGIN;
   x = s.data / n;
   y = s.data % n;
}
bool state_extend(const state_t &s, state_t &next) {
   while (action_cur < ACTION_END) {
      const int nx = x + dx[action_cur];
      const int ny = y + dy[action_cur];
      if (nx >= 0 && nx < m && ny >= 0 && ny < n && !map[nx][ny]) {
         next.data = nx * n + ny;
         if (!hashset_find(next)) {

            next.action = action_cur;
            next.father = state_hash(s);
            nodes[state_index(next)] = next;
            action_cur++;
            return true;
         }
      }
      action_cur++;
   }
   return false;
}
const state_t END = { 24, -1, -1 };
bool state_is_target(const state_t &s) {
   return s.data == END.data;
}

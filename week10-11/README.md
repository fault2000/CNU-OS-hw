# Purpose

 ping, pong을 순서대로 입력하는 게임을 구현하라
• server와 client 두 개의 프로그램으로 구성
• server와 client는 각자의 차례를 대기
• 각 프로그램은 점수 100점을 가지고 시작
• 실수한 경우 해당 프로그램은 점수 20점 감점
• server는 항상 ping, client는 항상 pong을 입력해야 함
• 각 프로그램 당 5회 반복한 후,남은 점수를 출력하면서 프로그램 종료
• 주의-무조건 server의 ping이 먼저 시작되어야 함
• 메모리 공유 기법(shared memory, FIFO 등)+Mutex(Mutex, semaphore 등) 필수 사용

# Build

server, client 실행 파일 만들기

```bash
make
```

사용 파일 정리

```bash
make clean
```

# Usage

각 명령은 다른 터미널에서 실행되어져야 함(주의: ./server < test_server.txt 부터 실행되어져야 함!!!)

```bash
./server < test_server.txt
```

```bash
./client < test_client.txt
```

만약 직접 해보고 싶다면, 각 터미널에서 아래 실행(주의: 위 ./server 부터 실행해야 함!!!)

```bash
./server
```

```bash
./client
```

이 후 server 부터 입력하여 시작.

# Chess Game

## 프로젝트 소개

본 프로젝트는 C 언어를 이용하여 구현한 콘솔 기반 체스 게임입니다.

사용자는 체스의 기본 규칙에 따라 기물을 이동할 수 있으며, 체크(Check), 체크메이트(Checkmate), 캐슬링(Castling), 폰 승진(Pawn Promotion) 등의 기능을 지원합니다.

Git과 GitHub를 활용하여 팀 프로젝트 형태로 개발하였습니다.

---

## 개발 환경

* Language : C
* IDE : Visual Studio Code
* Version Control : Git, GitHub

---

## 주요 기능

### 기물 이동

* Pawn
* Rook
* Knight
* Bishop
* Queen
* King

각 기물의 이동 규칙을 구현하였습니다.

### 체크(Check)

상대방의 킹이 공격받는 상태를 판정합니다.

### 체크메이트(Checkmate)

합법적인 이동이 존재하지 않을 경우 승패를 판정합니다.

### 캐슬링(Castling)

킹과 룩이 이동하지 않은 경우 캐슬링을 수행할 수 있습니다.

### 폰 승진(Pawn Promotion)

폰이 상대 진영 끝에 도달하면 다음 기물 중 하나로 승진할 수 있습니다.

* Queen
* Rook
* Bishop
* Knight

---

## 프로젝트 구조

```text
TeamProject-ChessGame
│
├── main.c
├── chessgame.c
├── chessgame.h
└── README.md
```

---

## 역할 분담

### 팀장

* 프로젝트 설계
* 메인 게임 루프 구현
* 사용자 입력 처리
* 게임 진행 및 턴 관리

### 팀원

* 기물 이동 규칙 구현
* 체크 및 체크메이트 판정
* 캐슬링 기능 구현
* 폰 승진 기능 구현
* 오류 수정 및 테스트

---

## GitHub Repository

Repository

https://github.com/minseo9630/TeamProject-ChessGame

---

## 실행 방법

컴파일

```bash
gcc main.c -o chess
```

실행

```bash
./chess
```

---

## 개발 기간

2026.05

---

## Team Project
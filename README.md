# 뱀 게임

이 프로젝트는 WinAPI를 사용하여 만든 뱀 게임입니다.

## 게임 설명

뱀 게임은 전통적인 아케이드 게임으로, 뱀을 조작하여 먹이를 먹고 자신의 몸통을 늘려가는 게임입니다. 

### 기능

- **뱀 조작**: 방향키를 사용하여 뱀을 이동시킵니다.
- **먹이 먹기**: 먹이를 먹으면 뱀의 길이가 늘어납니다.
- **벽 또는 자기 자신에 부딪히면 게임 종료**: 벽이나 뱀의 몸통에 부딪히면 게임이 종료됩니다.
- **점수 기록**: 먹은 먹이의 수로 점수를 기록합니다.

## 실행 방법

1. 해당 프로젝트를 클론합니다.
2. Visual Studio 이용해 프로젝트를 엽니다.
3. 솔루션을 빌드합니다.
4. 게임을 실행합니다.

## 게임 화면

 **시작(메안) 화면**
 ![image](https://github.com/sunbaklee/snake_game/assets/105528907/4b6f3fd6-4654-40f1-82d3-9542581fd8b8)

**게임 화면**
![image](https://github.com/sunbaklee/snake_game/assets/105528907/0e635f8d-6c6a-4c7b-aef4-7b4106743e6f)

**게임오버 화면**
![image](https://github.com/sunbaklee/snake_game/assets/105528907/87096084-cc55-489d-b7cd-bd79d99feb7c)

**랭킹 화면**


![image](https://github.com/sunbaklee/snake_game/assets/105528907/b40b2c10-e66a-403a-9d3c-53c14ba64f5c)


## 관계도

```mermaid
graph TD;
    A((WinMain)) -->|CreateWindow| B[WndProc메인화면];
    B --> |ShowSnakeInfo_no_quit| L[랭킹화면];
    B -->|startwindow게임시작| C[StartWndProc게임화면];
    C -->|WM_CREATE| D(GameInit);
    C -->|WM_PAINT| E(DrawGameBoard);
    C -->|WM_KEYDOWN| F(DirectControl);
    C -->|WM_TIMER| G(MovingWorm);
    C -->|WM_DESTROY| H(KillTimer, PostQuitMessage);
    F -->|DirectControl| G;
    G -->|MovingWorm| E;
    G -->|아이템 확인| K(ItemGenerator2);
    E -->|DrawGameBoard| C;
    D -->|ItemGenerator| E;
    G -->|충돌 확인| I(NewWindow게임오버 화면);
    I -->|ShowSnakeInfo| J(랭킹화면);
```

## 사용된 기술

- **WinAPI**: Windows용 응용 프로그램을 개발하기 위한 API를 사용했습니다.

## 저자

이 게임은 [sunbaklee]에 의해 만들어졌습니다.

## 최신버전

2023-11-28 버전


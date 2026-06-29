# NBC_CH3_Q5

게임 루프 및 UI 재설계 과제 프로젝트입니다.

## 개발 환경

* Unreal Engine 5.7.4
* C++
 
## 구현 내용

* 게임 시작 / 종료 흐름 구성
* Wave 기반 게임 진행 구조 구현
* 아이템 스폰 시스템 구현
* 코인 아이템 / 지뢰 아이템 구현
* 플레이어 체력 기반 게임 종료 처리
* UI 연동을 위한 이벤트 구조 구성
* Subsystem을 활용한 매니저 접근 구조 구현

## 주요 구조

```text
Source/CH3_Q5
├─ Game
├─ Wave
├─ Item
├─ Spawner
├─ Subsystem
├─ UI
└─ Interface
```

## 핵심 기능

### Game Flow

`GameManager`를 통해 게임 시작, 타이머 진행, 게임 종료 처리를 관리합니다.

### Wave

Wave 진행 상태를 관리하고, 게임 진행 흐름과 연결합니다.

### Item

공통 아이템 기반 클래스를 만들고, 코인 / 지뢰 아이템으로 기능을 분리했습니다.

### Spawn

SpawnVolume을 통해 아이템 생성 위치를 관리합니다.

### Subsystem

GameManager와 ItemSpawn 기능을 Subsystem으로 분리하여 접근성을 개선했습니다.


## 플로우 차트

<details>
 <summary>게임 진행 플로우</summary>
<img width="972" height="2311" alt="mermaid-diagram" src="https://github.com/user-attachments/assets/50587fe8-4c1e-4e81-aafa-9c2deb235a9d" />
</details>

<details>
 <summary>Wave 진행 플로우</summary>
<img width="429" height="1709" alt="mermaid-diagram (1)" src="https://github.com/user-attachments/assets/2b96f729-a6bf-4386-bc5e-1d12caf1bc8a" />
</details>

<details>
 <summary>UI 전환 플로우</summary>
<img width="803" height="724" alt="mermaid-diagram (2)" src="https://github.com/user-attachments/assets/ddb9bc42-84b3-4a2c-9efa-db692f451007" />
</details>

<details>
 <summary>클래스 다이어그램</summary>
<img width="1864" height="1480" alt="mermaid-diagram (3)" src="https://github.com/user-attachments/assets/1fec972d-ae5b-49fa-aa1a-e50d6b22178c" />
</details>


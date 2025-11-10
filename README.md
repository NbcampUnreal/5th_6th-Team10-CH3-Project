프로젝트 이름 : DungeonMaster

프로젝트 개요 : 던전에서 몬스터를 잡아서 골드를 버는 게임

주요 기능 요약 : 2개의 무기를 스왑하는 캐릭터와 던전 입장시 프리셋3개 중 랜덤으로 맵이 스폰되며 몬스터 AI가 플레이어를 추격하며 공격 밎 스폰 위치 기점 정찰 수행,
                보스 몬스터의 패턴 공격(델리게이트를 이용해서 플레이어의 골드 획득과 몬스터 사망 처리)

조작 방법 : wasd로 움직임 조작, 1번 근접무기 2번 원거리 무기, 왼쪽 쉬프트 회피, 마우스 좌클릭 공격

개발 환경 :
            -Engine : Unreal Engine 5.5
            -Language : C++
            -IDE : Visual Studio 2022
            -Compiler : MSVC
            -Build Tool : Unreal Build Systen(UBT)
            -Optional : CMake는 별도 사용하지 않음(언리얼 엔진에서 자동 빌드 관리)

빌드 / 실행 방법
            - *.uproject 파일을 더블 클릭하거나
              Unreal Editor -> "Open Project" 메뉴로 불러옵니다.
            
            - Visual Studio에서 빌드 
              솔루션 파일(.sln)을 Visual Studio 2022로 엽니다.
              상단의 Solution Configuration을 Development Editor,
              Solution Platform을 Win64로 설정합니다.
              Ctrl + Shift + B를 눌러 빌드합니다.
            
            -Unreal Editor 실행
              빌드 완료 후, Unreal Editor에서 Play(▶) 버튼을 눌러 실행합니다.
              또는 Visual Studio에서 F5 (디버그 실행) 로 실행할 수 있습니다.

            -패키징 (선택)
              상단 메뉴 → File > Package Project > Windows (64-bit) 선택
              빌드 완료 후 생성된 실행 파일(.exe)로 게임을 실행합니다.

핵심 로직 / 클래스 설명
            -캐릭터 : 컴뱃 컴포넌트에서 무기를 착용하고 스왑하는 시스템, 타이머 매니저를 이용한 회피를 통한 짧은 무적시간 구현
            -컴뱃컴포넌트 : UENUM을 이용한 무기 데이터 관리와 StartFire()함수 하나로 원거리 무기 일때 원거리 무기 공격과 애니메이션 재생, 근접 무기일때 근접 무기 공격과 애니메이션 재생.
            -게임매니저 : 몬스터와 캐릭터에게 델리게이트를 이용한 골드 습득 알림이나 죽음 처리, 랜덤 맵 스폰 시스템
            -몬스터AI : 스폰 위치 기반 랜덤 정찰 시스템, BTTaskNode를 추가해서 BT에서 필요한 노드 등록.

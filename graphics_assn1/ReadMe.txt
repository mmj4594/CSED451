Readme - CSED451 Computer Graphics assn1
Team GPS(20202405 문민재, 20202728 김진수)


프로젝트 디렉토리 설명
($ProjectDir)\bin - freeglut.dll, glew32.dll, 그리고 실행 파일(.exe)이 포함되어 있다.
($ProjectDir)\include - OpenGl과 GLM을 프로젝트에서 사용하기 위한 헤더 파일들이 들어있다.
($ProjectDir)\lib - freeglut, glew32의 라이브러리가 들어있다.
($ProjectDir)\x64\Debug - 디버그 관련 폴더이다.
($ProjectDir)\source.cpp - 이번 assn1의 소스 코드이다.
($ProjectDir)\source.h - 이번 assn1에서 사용된 헤더 파일이다. assn1의 구현에 사용된 전역변수, 클래스 등이 있다.


프로그램 실행 방법
프로젝트를 컴파일한 후 freeglut.dll과 glew32.dll이 있는 ($ProjectDir)\bin\x64 위치로 이동하여 graphics_assn1.exe를 실행시킨다.


게임 플레이 방법
벽이 도둑 및 플레이어를 향해 다가오고, 벽은 도둑과 부딪힐 경우 도둑의 색으로 변경되며
플레이어는 키보드를 입력하여 플레이어의 색을 정할 수 있고, 플레이어와 벽이 부딪힐 때 같은 색이어야 한다.
위 - 플레이어를 적색으로 바꾼다.
아래 - 플레이어를 녹색으로 바꾼다.
왼쪽 - 플레이어를 청색으로 바꾼다.
오른쪽 - 플레이어를 황색으로 바꾼다.
c - All pass 치트. 플레이어의 선택과 무관하게 무조건 pass로 간주한다.
f - All fail 치트. 플레이어의 선택과 무관하게 무조건 fail로 간주한다.
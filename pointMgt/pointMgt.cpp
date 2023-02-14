#include "winsock2.h"
#include "mysql.h"
#include "tchar.h"
#include<iostream>
#include<stdio.h>
#include <string.h>

using namespace std;
#pragma comment(lib, "libmysql.lib")
#pragma warning(disable:4996)

const char* host = "localhost";
const char* user = "root";
const char* pw = "2940";
const char* db = "point_db";

void DBconnect() {

	MYSQL* connection = NULL, conn;
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;

	mysql_init(&conn);
	connection = mysql_real_connect(&conn, host, user, pw, db, 3306, (char*)NULL, 0);
	mysql_query(&conn, "set names euckr");
	mysql_set_character_set(&conn, "euckr");

	if (connection == NULL) {
		printf("errno = %u\n", mysql_errno(&conn));
		printf("error message = %s\n", mysql_error(&conn));
	}
}

void finish_with_error(MYSQL* con) {
	fprintf(stderr, "%s\n", mysql_error(con));
	mysql_close(con);
}

void setcolor(int color, int bgcolor)
{
	color &= 0xf;
	bgcolor &= 0xf;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bgcolor << 4) | color);
}

void cin_only_num() {
	cin.clear();
	cin.ignore(INT_MAX, '\n');
	cout << "         숫자만 입력 가능합니다. 다시 입력해주세요! : ";
}

void table() {
	cout << endl << endl;
	cout << "         +-----------------------------------------------------------------------------------------+" << endl;
	cout << "         |     인적사항    |　　　　　　　　　　　　　　 상벌점 목록 　　　　　　　　　　　　　　　|" << endl;
	cout << "         +-----------------------------------------------------------------------------------------+" << endl;
	cout << "         |  학번  |  이름  | 두발 | 교복 | 화장 | 복도음식 | 금지 | 명찰 | 기타 |  상점  |   합계  |" << endl;
	cout << "         +-----------------------------------------------------------------------------------------+" << endl;
}

void createCard(MYSQL* conn) {

	MYSQL* connection = NULL;
	MYSQL_ROW sql_row;
	int hakbun, query_stat;
	char query[1024], name[50];

	system("cls"); cout << endl;
input:
	cout << "         학번 > ";
	while (!(cin >> hakbun)) {
		cin_only_num();
	}

	sprintf_s(query, "select * from point where hakbun = '%d';", hakbun);

	if (mysql_query(conn, query))
	{
		finish_with_error(conn);
	}

	MYSQL_RES* sql_result = mysql_store_result(conn);

	if (sql_result == NULL)
	{
		finish_with_error(conn);
	}

	if ((sql_row = mysql_fetch_row(sql_result))) {
		cout << endl;
		cout << "         동일한 학번이 존재하므로 생성이 불가능 합니다." << endl;
		goto input;
		getchar();
	}
	else {
		cout << "         이름 > ";
		cin >> name;

		sprintf_s(query, sizeof(query), "insert into point(hakbun, name) values('%d','%s')", hakbun, name);
		mysql_query(conn, query);

		cout << endl;
		cout << "         +--------------------+" << endl;
		cout << "           　학번 : " << hakbun << endl;
		cout << "           　이름 : " << name << endl;
		cout << "         +--------------------+" << endl;
		cout << "         벌점 카드 생성이 완료되었습니다." << endl << endl;

		sql_result = mysql_store_result(conn);
		mysql_free_result(sql_result);
		getchar();
	}
	getchar();
}

void deleteCard(MYSQL* conn) {

	MYSQL* connection = NULL;
	MYSQL_ROW sql_row;
	int hakbun;
	char query[1024], info;

	system("cls"); cout << endl;
input:
	cout << "         삭제할 학번 입력 > ";
	while (!(cin >> hakbun)) {
		cin_only_num();
	}

	sprintf_s(query, "select * from point where hakbun = '%d';", hakbun);

	if (mysql_query(conn, query))
	{
		finish_with_error(conn);
	}

	MYSQL_RES* sql_result = mysql_store_result(conn);

	if (sql_result == NULL)
	{
		finish_with_error(conn);
	}

	if ((sql_row = mysql_fetch_row(sql_result))) {
	alert:
		cout << "         정말 삭제 하시겠습니까? (Y/N) : ";
		cin >> info;

		if (info == 'Y' || info == 'y') {
			sprintf_s(query, sizeof(query), "delete from point where hakbun like '%d';", hakbun);
			mysql_query(conn, query);
			cout << "         삭제가 완료 되었습니다." << endl;
			getchar();
		}
		else if (info == 'N' || info == 'n') {
			cout << "         삭제를 취소하였습니다." << endl;
			getchar();
		}
		else {
			cout << "         존재하지 않는 기능 입니다." << endl << endl;
			getchar();
			goto alert;
		}
	}
	else {
		cout << "         존재하지 않는 학번 입니다." << endl << endl;
		goto input;
	}
	getchar();
}

void updatePoint(MYSQL* conn) {

	MYSQL* connection = NULL;
	MYSQL_ROW sql_row;
	int hakbun, no, num, input, point, sum;
	char query[1024];

	system("cls"); cout << endl;
input:
	cout << "         업데이트 대상 학번 입력 > ";
	while (!(cin >> hakbun)) {
		cin_only_num();
	}

	sprintf_s(query, "select * from point where hakbun = '%d';", hakbun);

	if (mysql_query(conn, query))
	{
		finish_with_error(conn);
	}

	MYSQL_RES* sql_result = mysql_store_result(conn);

	if (sql_result == NULL)
	{
		finish_with_error(conn);
	}

	if ((sql_row = mysql_fetch_row(sql_result))) {
		cout << endl;
	mode:
		cout << "         +--------------------+" << endl;
		cout << "         | ① 벌점 관리 모드  |" << endl;
		cout << "         | ② 상점 관리 모드  |" << endl;
		cout << "         +--------------------+" << endl;
		cout << "         번호 입력 > ";
		while (!(cin >> num)) {
			cin_only_num();
		}

		sprintf_s(query, sizeof(query), "select * from point where hakbun = '%d';", hakbun);
		mysql_query(conn, query);

		if (num == 1) {
			cout << endl;
			cout << "         +-------------------------------------------------------------+" << endl;
			cout << "         |　① 두발 불량 | ② 교복 불량 | ③ 화장 | ④ 복도 음식섭취 　|" << endl;
			cout << "         |　　　　  ⑤ 금지 반입물 | ⑥ 명찰 미착용 | ⑦ 기타  　　　　|" << endl;
			cout << "         +-------------------------------------------------------------+" << endl;
			cout << "         업데이트 항목 번호 입력 > ";
			while (!(cin >> no)) {
				cin_only_num();
			}
			cout << "         횟수 입력 > ";
			while (!(cin >> input)) {
				cin_only_num();
			}

			point = 0;
			if (no == 1) { point = input * -2; }
			else if (no == 2) { point = input * -1; }
			else if (no == 3) { point = input * -4; }
			else if (no == 4) { point = input * -1; }
			else if (no == 5) { point = input * -3; }
			else if (no == 6) { point = input * -1; }
			else if (no == 7) { point = input * -1; }

			sprintf_s(query, sizeof(query), "select * from point where hakbun='%d';", hakbun);
			mysql_query(conn, query);

			sum = 0;
			sql_result = mysql_store_result(conn);
			while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
				string s = sql_row[no + 1];
				string tmpSum = sql_row[10];
				point += atoi(s.c_str());
				sum = atoi(tmpSum.c_str()) + (point - atoi(s.c_str()));
			}
			mysql_free_result(sql_result);

			sprintf_s(query, sizeof(query), "update point set p%d='%d', sum='%d' where hakbun='%d';", no, point, sum, hakbun);
			mysql_query(conn, query);

			sprintf_s(query, sizeof(query), "select * from point where hakbun='%d';", hakbun);
			mysql_query(conn, query);

			sql_result = mysql_store_result(conn);
			table();
			while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
				printf("           %s    %s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t+%s\t%s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4], sql_row[5], sql_row[6], sql_row[7], sql_row[8], sql_row[9], sql_row[10]);
				cout << "         -------------------------------------------------------------------------------------------" << endl;
			}
			cout << "         업데이트가 성공적으로 완료되었습니다." << endl;
			mysql_free_result(sql_result);
		}
		else if (num == 2) {
			cout << endl;
			cout << "         +-------------------------------------+" << endl;
			cout << "         |    ① 교내 봉사  |  ② 교내 수상    |" << endl;
			cout << "         +-------------------------------------+" << endl;
			cout << "         상점 추가 항목 번호 입력 > ";
			while (!(cin >> no)) {
				cin_only_num();
			}
			cout << "         횟수 입력 > ";
			while (!(cin >> input)) {
				cin_only_num();
			}

			point = 0;
			if (no == 1) { point = input * 5; }
			else if (no == 2) { point = input * 10; }

			sprintf_s(query, sizeof(query), "select * from point where hakbun='%d';", hakbun);
			mysql_query(conn, query);

			sum = 0;
			sql_result = mysql_store_result(conn);
			while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
				string s = sql_row[9];
				string tmpSum = sql_row[10];
				point += atoi(s.c_str());
				sum = atoi(tmpSum.c_str()) + (point - atoi(s.c_str()));
			}
			mysql_free_result(sql_result);

			sprintf_s(query, sizeof(query), "update point set p8='%d', sum='%d' where hakbun='%d';", point, sum, hakbun);
			mysql_query(conn, query);

			sprintf_s(query, sizeof(query), "select * from point where hakbun='%d';", hakbun);
			mysql_query(conn, query);

			sql_result = mysql_store_result(conn);
			table();
			while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
				printf("           %s    %s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t+%s\t%s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4], sql_row[5], sql_row[6], sql_row[7], sql_row[8], sql_row[9], sql_row[10]);
				cout << "         -------------------------------------------------------------------------------------------" << endl;
			}
			cout << "         업데이트가 성공적으로 완료되었습니다." << endl;
			mysql_free_result(sql_result);
		}
		else {
			cout << "         존재하지 않는 기능 입니다." << endl << endl;
			goto mode;
		}
	}
	else {
		cout << "         존재하지 않는 학번 입니다." << endl;
		goto input;
	}
	getchar(); getchar();
}

void idSearch(MYSQL* conn) {

	MYSQL* connection = NULL;
	MYSQL_ROW sql_row;
	int hakbun;
	char query[1024];

	system("cls"); cout << endl;
input:
	cout << "         검색할 학번 입력 > ";
	while (!(cin >> hakbun)) {
		cin_only_num();
	}

	sprintf_s(query, "select * from point where hakbun = '%d';", hakbun);

	if (mysql_query(conn, query))
	{
		finish_with_error(conn);
	}

	MYSQL_RES* sql_result = mysql_store_result(conn);

	if (sql_result == NULL)
	{
		finish_with_error(conn);
	}

	if ((sql_row = mysql_fetch_row(sql_result))) {
		sprintf_s(query, sizeof(query), "select * from point where hakbun = '%d';", hakbun);
		mysql_query(conn, query);

		sql_result = mysql_store_result(conn);
		table();
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
			printf("           %s    %s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t+%s\t%s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4], sql_row[5], sql_row[6], sql_row[7], sql_row[8], sql_row[9], sql_row[10]);
			cout << "         -------------------------------------------------------------------------------------------" << endl;
		}
		cout << "         조회가 성공적으로 완료되었습니다." << endl;
		mysql_free_result(sql_result);
	}
	else {
		cout << "         존재하지 않는 학번 입니다." << endl << endl;
		goto input;
	}
	getchar(); getchar();
}

void nameChange(MYSQL* conn) {

	MYSQL* connection = NULL;
	MYSQL_ROW sql_row;
	int hakbun, query_stat;
	char query[1024], name[50];

	system("cls"); cout << endl;
input:
	cout << "         이름 변경할 학번 입력 > ";
	while (!(cin >> hakbun)) {
		cin_only_num();
	}

	sprintf_s(query, "select * from point where hakbun = '%d';", hakbun);

	if (mysql_query(conn, query))
	{
		finish_with_error(conn);
	}

	MYSQL_RES* sql_result = mysql_store_result(conn);

	if (sql_result == NULL)
	{
		finish_with_error(conn);
	}

	if ((sql_row = mysql_fetch_row(sql_result))) {
		cout << "         변경할 이름 > ";
		cin >> name;

		sprintf_s(query, sizeof(query), "update point set name='%s' where hakbun='%d';", name, hakbun);
		mysql_query(conn, query);
		sprintf_s(query, sizeof(query), "select * from point where hakbun='%d';", hakbun);
		mysql_query(conn, query);

		sql_result = mysql_store_result(conn);
		table();
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
			printf("           %s    %s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t+%s\t%s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4], sql_row[5], sql_row[6], sql_row[7], sql_row[8], sql_row[9], sql_row[10]);
			cout << "         -------------------------------------------------------------------------------------------" << endl;
		}
		cout << "         업데이트가 성공적으로 완료되었습니다." << endl;
		mysql_free_result(sql_result);
		getchar();
	}
	else {
		cout << "         존재하지 않는 학번 입니다." << endl << endl;
		goto input;
		getchar();
	}
	getchar();
}

void pointInfo() {

	system("cls"); cout << endl << endl;
	cout << "　　　　　　벌점과 상점은 상황에 따라 점수가 다르게 부여될 수 있습니다." << endl;
	cout << "　　　　　　　　　　  아래 목록표를 참고해주시길 바랍니다." << endl << endl;
	cout << "　　 　　　　+-------------------------+ +-------------------------+" << endl;
	cout << "　　　　　　          벌점 목록                   상점 목록           " << endl;
	cout << "　　　　　　 +-------------------------+ +-------------------------+" << endl;
	cout << "　　　　　　　　　 두발 불량 : -2점　　   　　　교내 봉사 : 1점　　 " << endl;
	cout << "　　　　　　　　　 교복 불량 : -1점　　　　　　 교내 수상 : 2점　　 " << endl;
	cout << "　　　　　　　　　　  화장　 : -4점　　   " << endl;
	cout << "　　　　　　　　 복도 음식물 : -1점　　   " << endl;
	cout << "　　　　　　　　 금지 반입물 : -3점　   　" << endl;
	cout << "　　　　　　　　 명찰 미착용 : -1점　　   " << endl;
	cout << "　　　　　　　　　　  기타　 : -1점　　   " << endl << endl << endl;
	getchar();
}

void sortSelect(MYSQL* conn) {

	MYSQL* connection = NULL;
	MYSQL_ROW sql_row;
	MYSQL_RES* sql_result = mysql_store_result(conn);
	int num;
	char query[1024];

	system("cls"); cout << endl;
input:
	cout << "         +--------------------+" << endl;
	cout << "         |　　 1) 이름순  　　|" << endl;
	cout << "         |　　 2) 학번순  　　|" << endl;
	cout << "         |　　 3) 누적순  　　|" << endl;
	cout << "         +--------------------+" << endl;
	cout << "         정렬 방법 선택 > ";
	while (!(cin >> num)) {
		cin_only_num();
	}

	if (num == 1) {
		sprintf_s(query, "select * from point order by name;");
	}
	else if (num == 2) {
		sprintf_s(query, "select * from point order by hakbun;");
	}
	else if (num == 3) {
		sprintf_s(query, "select * from point order by sum desc;");
	}
	else {
		cout << "         존재하지 않는 기능 입니다." << endl << endl;
		goto input;
	}
	mysql_query(conn, query);
	sql_result = mysql_store_result(conn);

	table();
	while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
		printf("           %s    %s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t+%s\t%s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4], sql_row[5], sql_row[6], sql_row[7], sql_row[8], sql_row[9], sql_row[10]);
		cout << "         -------------------------------------------------------------------------------------------" << endl;
	}
	cout << "         조회가 성공적으로 완료되었습니다." << endl;
	mysql_free_result(sql_result);
	getchar(); getchar();
}

int _tmain(int argc, _TCHAR* argv[]) {

	MYSQL* connection;
	MYSQL conn;
	MYSQL_RES* sql_result = NULL;
	//MYSQL_ROW sql_row;

	int num = 1, hakbun;
	char password[50];

	if (mysql_init(&conn) == NULL) {
		printf("mysql_init() error!");
	}

	connection = mysql_real_connect(&conn, host, user, pw, db, 3306, (const char*)NULL, 0);
	mysql_set_character_set(&conn, "euckr");

	if (connection == NULL) {
		printf("errno = %u\n", mysql_errno(&conn));
		printf("error message = %s\n", mysql_error(&conn));
		return 1;
	}
	else {
		DBconnect();
		setcolor(14, 0);
		cout << endl;
		cout << "       ┏─…─…─…─…─…─…─…─…─…─…─…─…─…─…─…─…─…─…─…─…─┓" << endl;
		cout << "       ┃ *   ┌━━━┐┌━━━┐┌━━━┐┌━━━┐┌━━━┐┌━━━┐┌━━━┐┌━━━┐┌━━━┐┌━━━┐     *┃" << endl;
		cout << "       ┃ *   │ 벌││ 점││ 카││ 드││ 관││ 리││ 프││ 로││ 그││ 램│     *┃" << endl;
		cout << "       ┃ *   └━━━┘└━━━┘└━━━┘└━━━┘└━━━┘└━━━┘└━━━┘└━━━┘└━━━┘└━━━┘     *┃" << endl;
		cout << "       ┗─…─…─…─…─…─…─…─…─…─…─…─…─…─…─…─…─…─…─…─…─┛" << endl;
		setcolor(15, 0);

		while (num != 0) {
		mode:
			cout << endl;
			cout << "         사용자 모드를 선택해주세요." << endl;
			cout << "         +--------------------+" << endl;
			cout << "         | ① 학생 모드       |" << endl;
			cout << "         | ② 관리자 모드     |" << endl;
			cout << "         +--------------------+" << endl;
			cout << "         번호 입력 (종료: 0) > ";
			while (!(cin >> num)) {
				cin_only_num();
			}

			if (num == 0) {
				cout << "         프로그램을 종료합니다." << endl; exit(0);  break;
			}
			else if (num == 1) {
				system("cls"); cout << endl;

				while (num != 4) {
					system("cls"); cout << endl;
					cout << "         *** 환영합니다! 학생은 벌점 및 상점 조회만 가능합니다. ***" << endl << endl;
				menu:
					cout << "         +------------------------+" << endl;
					cout << "         | ① 학번 검색           |" << endl;
					cout << "         | ② 벌점 및 상점 안내   |" << endl;
					cout << "         | ③ 사용자 모드 변경    |" << endl;
					cout << "         +------------------------+" << endl;
					cout << "          번호 입력 (종료: 0) > ";
					while (!(cin >> num)) {
						cin_only_num();
					}

					switch (num) {
					case 0: cout << "         프로그램을 종료합니다." << endl; exit(0);  break;
					case 1: idSearch(connection); break;
					case 2: pointInfo();	break;
					case 3: goto mode;  break;
					default:  cout << "         존재하지 않는 기능 입니다." << endl << endl;  goto menu;
					}
				}
			}
			else if (num == 2) {
				system("cls"); cout << endl;
				cout << "         *** 환영합니다! 프로그램 보안을 위해 관리자 확인이 필요합니다. ***" << endl << endl;
			info:
				cout << "         관리자 비밀번호 입력 > ";
				cin >> password;

				if (!strcmp(password, "2940")) {
					cout << "         로그인 되었습니다. 잠시만 기다려주세요..." << endl;
					Sleep(1000);

					while (num != 8) {
						system("cls"); cout << endl;
					admin:
						cout << "         +------------------------+" << endl;
						cout << "         | ① 벌점카드 생성       |" << endl;
						cout << "         | ② 벌점카드 삭제       |" << endl;
						cout << "         | ③ 벌점 업데이트       |" << endl;
						cout << "         | ④ 목록 조회           |" << endl;
						cout << "         | ⑤ 학번 검색           |" << endl;
						cout << "         | ⑥ 이름 변경           |" << endl;
						cout << "         | ⑦ 사용자 모드 변경    |" << endl;
						cout << "         +------------------------+" << endl;
						cout << "         번호 입력 (종료: 0) > ";
						while (!(cin >> num)) {
							cin_only_num();
						}

						switch (num) {
						case 0: cout << "         프로그램을 종료합니다." << endl; exit(0);  break;
						case 1: createCard(connection);	  break;
						case 2: deleteCard(connection);	  break;
						case 3: updatePoint(connection);	  break;
						case 4: sortSelect(connection);		  break;
						case 5: idSearch(connection);		  break;
						case 6: nameChange(connection); break;
						case 7: goto mode;	break;
						default:  cout << "         존재하지 않는 기능 입니다." << endl << endl;  goto admin;
						}
					}
				}
				else {
					cout << "         비밀번호가 일치하지 않습니다. 다시 입력해주세요!" << endl;
					goto info;
				}
			}
			else {
				cout << "         존재하지 않는 기능 입니다." << endl << endl;
				goto mode;
			}
		}
	}
}
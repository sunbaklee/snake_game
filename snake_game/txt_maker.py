import pymysql

# MariaDB 연결 정보
host = 'svc.sel5.cloudtype.app'
port = 32085
user = 'root'
password = 'toor'

try:
    # MariaDB 연결
    connection = pymysql.connect(
        host=host,
        port=port,
        user=user,
        password=password,
        database='snake'  # snake 데이터베이스 선택
    )
    
    # 연결 성공 시 메시지 출력
    print("연결 성공!")
    
    # 커서 생성
    cursor = connection.cursor()
    
    # SQL 쿼리 실행 (내림차순 정렬)
    query = "SELECT name, score FROM info ORDER BY score DESC"
    cursor.execute(query)
    
    # 결과 가져오기
    rows = cursor.fetchall()
    
    # 파일로 데이터 저장
    with open('snake_info.txt', 'w') as file:
        for row in rows:
            name, score = row
            file.write(f"Name: {name}, Score: {score}\n")
    
    # 연결 종료
    connection.close()

    print("내림차순으로 데이터를 파일로 저장했습니다: snake_info.txt")

except pymysql.Error as e:
    # 연결 실패 시 에러 메시지 출력
    print(f"연결 실패: {e}")

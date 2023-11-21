import sys
import pymysql

# Check if the correct number of arguments is provided
if len(sys.argv) != 3:
    print("Usage: python script_name.py <name> <score>")
    sys.exit(1)

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
    
    # 사용자로부터 이름 입력 받기
    name = sys.argv[1]
    
    # 점수 파라미터로 받아오기
    score = int(sys.argv[2])
    
    # 커서 생성
    cursor = connection.cursor()
    
    # SQL 쿼리 실행 (데이터 삽입)
    insert_query = "INSERT INTO info (name, score) VALUES (%s, %s)"
    cursor.execute(insert_query, (name, score))
    
    # 변경사항 커밋
    connection.commit()
    
    # 연결 종료
    connection.close()

    print("데이터를 데이터베이스에 추가했습니다.")

except pymysql.Error as e:
    # 연결 실패 시 에러 메시지 출력
    print(f"연결 실패: {e}")

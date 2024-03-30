import datetime
from datetime import datetime

# 获取当前时间
current_time = datetime.now()

# 格式化输出为YYYYMMDDHHMMSS
formatted_time = current_time.strftime("%Y%m%d%H%M%S")

print(formatted_time)

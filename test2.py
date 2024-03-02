from requests import post
from json import dumps
import os

# 函数：查询C或C++语言提交记录
def query_submissions(problem_display_id, code_language="c", locale="zh_CN", take_count=10):
    url = "https://api.loj.ac/api/submission/querySubmission"
    headers = {"Content-Type": "application/json"}
    data = dumps({
        "problemDisplayId": problem_display_id,
        "codeLanguage": code_language,  # 可以是"c"或"cpp"，根据参数决定
        "status": "Accepted",
        "locale": locale,
        "takeCount": take_count
    })

    response = post(url, headers=headers, data=data)
    if response.status_code in [200, 201]:
        submissions = response.json().get('submissions', [])
        return [sub["id"] for sub in submissions]
    else:
        print(f"查询{code_language}语言提交记录失败: {response.status_code}, {response.text}")
        return []

# 函数：获取提交详情
def get_submission_detail(submission_id, locale="zh_CN"):
    url = "https://api.loj.ac/api/submission/getSubmissionDetail"
    headers = {"Content-Type": "application/json"}
    data = dumps({
        "submissionId": str(submission_id),
        "locale": locale
    })

    response = post(url, headers=headers, data=data)
    if response.status_code in [200, 201]:
        return response.json()
    else:
        print(f"获取提交{submission_id}详情失败: {response.status_code}, {response.text}")
        return None

# 函数：保存代码为.c或.cpp文件
def save_code(problem_id, submission_detail, language):
    if submission_detail:
        submission_id = submission_detail["meta"]["id"]
        code = submission_detail["content"]["code"]
        problem_dir = f"problem_{problem_id}"
        submissions_dir = os.path.join(problem_dir, f"submission{'' if language == 'c' else '++'}")
        os.makedirs(submissions_dir, exist_ok=True)
        file_extension = '.c' if language == 'c' else '.cpp'
        file_path = os.path.join(submissions_dir, f"{submission_id}{file_extension}")
        with open(file_path, "w", encoding='utf-8') as file:
            file.write(code)
        print(f"代码已保存为 {file_path}。")
    else:
        print("没有提供详细信息以保存代码。")

# 主程序
if __name__ == "__main__":
    for problem_id in range(1, 1001):  # 遍历问题1到1000
        for language in ['c', 'cpp']:  # 对每种语言执行操作
            submission_ids = query_submissions(problem_id, code_language=language)
            for submission_id in submission_ids:
                submission_detail = get_submission_detail(submission_id)
                save_code(problem_id, submission_detail, language)
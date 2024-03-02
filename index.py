from requests import post, get
from json import dumps, loads
import os

# 记录脚本启动时的工作目录
initial_working_directory = os.getcwd()

def get_problem_meta(id):
    response = post("https://api.loj.ac/api/problem/getProblem", headers={
        "Content-Type": "application/json"
    }, data=dumps({
        "displayId": id,
        "testData": True,
        "additionalFiles": True,
        "localizedContentsOfLocale": "zh_CN",
        "samples": True
    })).json()
    return response

def get_data_url(file_name_list, id):
    # 如果文件名列表为空，则直接返回空列表，避免不必要的API调用
    if not file_name_list:
        return []

    response = post("https://api.loj.ac/api/problem/downloadProblemFiles", headers={
        "Content-Type": "application/json"
    }, data=dumps({
        "problemId": id,
        "type": "TestData",
        "filenameList": file_name_list
    })).json()

    # 检查 'downloadInfo' 字段是否存在
    if 'downloadInfo' not in response:
        print(f"问题 {id} 的测试数据下载信息缺失，跳过下载测试数据。")
        return []
    else:
        return response['downloadInfo']

def download_problem(id):
    global initial_working_directory
    dat = get_problem_meta(id)
    
    # 检查 'localizedContentsOfLocale' 和 'testData' 是否存在
    if 'localizedContentsOfLocale' not in dat or 'contentSections' not in dat['localizedContentsOfLocale'] or 'testData' not in dat:
        print(f"问题 {id} 缺少 'localizedContentsOfLocale', 'contentSections' 或 'testData' 字段，跳过...")
        return
    
    # 数据检查通过后，创建问题目录
    problem_dir = os.path.join(initial_working_directory, str(id))
    os.makedirs(problem_dir, exist_ok=True)
    os.chdir(problem_dir)

    with open("Description.md", "w+", encoding="utf-8") as f:
        content = dat["localizedContentsOfLocale"]["contentSections"]
        for i in content:
            f.write("## " + i["sectionTitle"] + "\n")
            if i["type"] == 'Text':
                f.write(i["text"] + "\n")
            elif i["type"] == "Sample":
                f.write("### Input\n```\n" + dat["samples"][i["sampleId"]]["inputData"] + "```\n")
                f.write("### Output\n```\n" + dat["samples"][i["sampleId"]]["outputData"] + "```\n")

    # 检查是否有测试数据可下载
    testData = dat["testData"]
    fnlist = [i["filename"] for i in testData]
    if fnlist:
        os.makedirs("testData", exist_ok=True)
        os.chdir("testData")
        URList = get_data_url(fnlist, id)
        if URList:  # 有测试数据可下载
            for i in URList:
                resp = get(i["downloadUrl"])
                with open(i["filename"], "wb+") as f:
                    f.write(resp.content)
        os.chdir("..")
    else:
        print(f"问题 {id} 没有可下载的测试数据。")

    os.chdir(initial_working_directory)  # 操作完成后，确保返回初始目录

def query_c_submissions(problem_display_id, locale="zh_CN", take_count=10):
    url = "https://api.loj.ac/api/submission/querySubmission"
    data = dumps({
        "problemDisplayId": problem_display_id,
        "codeLanguage": "c",
        "status": "Accepted",
        "locale": locale,
        "takeCount": take_count
    })
    response = post(url, headers={"Content-Type": "application/json"}, data=data)
    if response.status_code in [200, 201]:
        submissions = response.json().get('submissions', [])
        return [sub["id"] for sub in submissions]
    else:
        print(f"查询提交记录失败: {response.status_code}, {response.text}")
        return []

def get_submission_detail(submission_id, locale="zh_CN"):
    url = "https://api.loj.ac/api/submission/getSubmissionDetail"
    data = dumps({
        "submissionId": str(submission_id),
        "locale": locale
    })
    response = post(url, headers={"Content-Type": "application/json"}, data=data)
    if response.status_code in [200, 201]:
        return response.json()
    else:
        print(f"获取提交{submission_id}详情失败: {response.status_code}, {response.text}")
        return None

def save_code(submission_detail, problem_id):
    if submission_detail:
        submission_id = submission_detail["meta"]["id"]
        code = submission_detail["content"]["code"]
        submissions_dir = os.path.join(str(problem_id), "submission")
        os.makedirs(submissions_dir, exist_ok=True)
        file_path = os.path.join(submissions_dir, f"{submission_id}.c")
        with open(file_path, "w", encoding='utf-8') as file:
            file.write(code)
        print(f"代码已保存为 {file_path}。")
    else:
        print("没有提供详细信息以保存代码。")

if __name__ == "__main__":
    for problem_id in range(515, 1001):
        download_problem(problem_id)
        os.chdir(initial_working_directory)  # 在查询提交之前回到初始目录
        submission_ids = query_c_submissions(problem_id)
        for submission_id in submission_ids:
            submission_detail = get_submission_detail(submission_id)
            save_code(submission_detail, problem_id)
        os.chdir(initial_working_directory)  # 操作完成后返回初始目录

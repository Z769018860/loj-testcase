# loj-testcase
loj上爬取的题目、测试用例和C/C++的提交记录

## 使用方式
直接运行index.py
```
python index.py
```
会自动爬取1-1000的问题的问题描述、测试用例文件和提交记录中C语言的10个accepted代码，自动保存在对应题目序号内、
ps：因为测试用例下载很慢，所以要跑很久。需要特定问题可以直接更改index.py文件中的问题区间range[1,1000]
如果只想要代码，可以进入test文件夹，执行
```
python test.py
```
这个会自动爬取1-1000的问题的C和C++代码，每个问题爬取10个。
结果会保存在对应题目文件夹里，submission为C语言的代码，submission++是C++的代码
之所以写了这个test.py是因为后续题目C语言的代码真的太少了……

## 数据说明
都是网站上公开的数据，不登录也可以获取，不同时间爬取可能得到的代码记录不一样
需要使用可以直接在题号文件夹里找到提交编号
需要爬取其他语言的代码或者某一题的更多代码可以修改里面的几个参数。

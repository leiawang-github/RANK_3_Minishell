import re, sys, requests

def check_qzone(qq):
    url = f"https://user.qzone.qq.com/{qq}"
    try:
        r = requests.get(url, timeout=8, headers={
            "User-Agent":"Mozilla/5.0"
        })
    except Exception as e:
        return f"{qq}\t网络异常：{e}"

    html = r.text
    if re.search(r"仅好友可见|权限不足|需登录|没有访问权限", html):
        return f"{qq}\t非公开/受限"
    if re.search(r"说说|日志|相册|访客|留言板", html):
        return f"{qq}\t可能公开"
    # 有些页面会用JS加载，静态HTML很干净
    if r.status_code == 200:
        return f"{qq}\t未知（可能需登录或JS渲染）"
    return f"{qq}\tHTTP {r.status_code}"

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("用法: python qzone_check.py 123456789 [更多QQ号...]")
        sys.exit(1)
    for qq in sys.argv[1:]:
        print(check_qzone(qq))

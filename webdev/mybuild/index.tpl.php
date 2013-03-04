<!doctype html>
<html>
    <head>
        <title>标签系统 -- 首页</title>
        <meta charset="utf-8" />
        <link href="/styles/admin.css" rel="stylesheet" />
        <link rel="stylesheet" href="http://ajax.googleapis.com/ajax/libs/jqueryui/1.9.2/themes/base/jquery-ui.css" />
        <script src="http://ajax.googleapis.com/ajax/libs/jquery/1.8.3/jquery.min.js"></script>  
        <script src="http://ajax.googleapis.com/ajax/libs/jqueryui/1.9.2/jquery-ui.min.js"></script>
    </head>
    <body>
        <div class="container">
            <div id="header">
                <div class="wrapper">
                    <div class="logo"></div>
                    <ul id="nav">
                        <li>
                            <a href="/admin/index.php">首页</a>
                        </li>
                    </ul>
                </div>
            </div>
            <div id="main">
                <div id="sidebar"></div>
                <div id="content">
                    <h1>功能列表</h1>
                    <ul class="content">
                        <li>
                            <a href="/admin/article.php">文章信息更新</a>
                        </li>
                        <li>
                            <a href="/admin/tag_counts.php">标签包含文章排行</a>
                        </li>
                    </ul>
                </div>
            </div>
            <div id="footer">
            </div>
        </div>
    </body>
</html>

# QJson解析json格式   

## 1 QJson的介绍  
- Qt中对JSON支持提供了一个易于使用的c++ API来解析、修改和保存JSON数据。它还支持以二进制格式保存这些数据，这种格式是直接“mmap”(一种内存映射文件的方法)的，并且访问起来非常快。    

## 2 QJson的使用   
## 2.1 创建Json格式   
```C++
// 声明解析Json的头文件
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

 // 1. 创建数组 append
    QJsonArray versionArray;
    versionArray.append(4.8);
    versionArray.append(5.2);
    versionArray.append(5.7);

    // 2. 创建对象  insert
    QJsonObject pageObject;
    //                属性名    属性值
    pageObject.insert("Home", "https://www.qt.io/");
    pageObject.insert("Download", "https://www.qt.io/download/");
    pageObject.insert("Developers", "https://www.qt.io/developers/");

    // 3. 组合Json
    QJsonObject json;
    json.insert("Name", "Qt");
    json.insert("Company", "Digia");
    json.insert("From", 1991);
    json.insert("Version", QJsonValue(versionArray));
    json.insert("Page", QJsonValue(pageObject));

    // 4. Json 格式的转换  QJsonDocument
    QJsonDocument QJsonDoc;
    QJsonDoc.setObject(json);
    qDebug().noquote() << "OutJson: \n"<< json;

    // 5. QJsonDocument  转换成 QByteArray
    QByteArray byteArray = QJsonDoc.toJson(QJsonDocument::Compact);

    // 6. QByteArray 转换成 QSting
    QString strJson(byteArray);

    // 7. 输出QSting
    qDebug().noquote() << "OutQStringJson :" << strJson;
```

## 3 Json格式的修改   
```C++
   QJsonObject JsonData;
    JsonData.insert("Name", "Qt");
    JsonData.insert("From", 1991);
    JsonData.insert("Cross Platform", true);

    QJsonObject CS;
    CS.insert("CS", JsonData);
    QJsonObject CCS;
    CCS.insert("CCS", CS);


    QJsonObject root;
    root.insert("root1", CCS);
    root.insert("root2", "test");

    // 构建 JSON 文档
    QJsonDocument document;
    document.setObject(root);
    // 转成 QByteArray 再转成  QString
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);
    qDebug().noquote() << "rootString:" << strJson;

    // Json格式的修改：
    // 1. 修改没有嵌套的对象，直接修改,但是，嵌套过他的没修改
    JsonData["Name"] = "99";
    qDebug() << "JsonData" << JsonData;
    qDebug() << "Jsonroot" <<  root;

    // 2. 修改嵌套最深处，需要分页修改
    // 思路： 多层结构需要分页查找，先找最外层，将查找到的内层，
    // 再转成Json对象，直到最内层
    // 2.1-1 第一层页面的引用
    QJsonValueRef RefPage1 = root.find("root1").value();
    // 2.1-2 将第一层页面的引用，转成Json对象
    QJsonObject root1obj = RefPage1.toObject();

    // 2.2-1 第二层页面的引用
    QJsonValueRef RefPage2 = root1obj.find("CCS").value();
    // 2.2-2 将第二层页面的引用，转成Json对象
    QJsonObject CCSobj = RefPage2.toObject();

    // 2.3-1 第三层页面的引用
    QJsonValueRef RefPage3 = CCSobj.find("CS").value();
    // 2.3-2 将第二层页面的引用，转成Json对象
    QJsonObject CSobj = RefPage3.toObject();


    // 2.4 将内层要修改的地方修改
    CSobj["From"] = 1995;

    // 2.5 对照 2.3-2 反向赋值
    RefPage3 = CSobj;

    // 2.6 对照 2.2-2 反向赋值
    RefPage2 = CCSobj;

    // 2.7 对照 2.1-2 反向赋值
    RefPage1 = root1obj;

    qDebug().noquote() << "Jsonroot:" << root;

```

## 3 Json与QString的转换  
```C++
QJsonObject MainWindow::QStringToJson(QString jsonString)
{
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toLocal8Bit().data());
    if(jsonDocument.isNull())
    {
        qDebug()<< "String NULL"<< jsonString.toLocal8Bit().data();
    }
    QJsonObject jsonObject = jsonDocument.object();
    return jsonObject;
}

QString MainWindow::JsonToQString(QJsonObject jsonObject)
{
    return QString(QJsonDocument(jsonObject).toJson());
}

```

## 4 Json和QByteArray的相互转换
```C++
// 1. QJsonObject 转成  QJsonDocument 再转  QByteArray
QJsonObject json;
QByteArray ary;
QJsonDocument doc(json);
ary= doc.toJson();
   
// 2. QByteArray 转成  QJsonDocument 再转 QJsonObject 
QByteArray ary;
QJsonObject json;
json = QJsonDocument::fromJson(ary).object();    
```

## 5 输出打印属性值    

```C++
void MainWindow::AnalysisJsonByQJson()
{
    QJsonArray json_array2 = QJsonDocument::fromJson(m_byteArray).array();
    int tag = json_array2.at(0).toInt();

    QJsonObject json_object2 = QJsonDocument::fromJson(m_byteArray).object();
    qDebug() << json_object2.value("root2").toString();
    QJsonObject page1 = json_object2.value("root1").toObject();

    QJsonObject CCS = page1.value("CCS").toObject();

    QJsonObject CS = CCS.value("CS").toObject();

    qDebug() << CS.value("Name").toString();
    qDebug() << CS.value("From").toInt();
    qDebug() << CS.value("Cross Platform").toBool();
}
```





## 6 参考资料  
1. https://blog.csdn.net/kenfan1647/article/details/107596675?utm_medium=distribute.pc_relevant_bbs_down.none-task-blog-baidujs-1.nonecase&depth_1-utm_source=distribute.pc_relevant_bbs_down.none-task-blog-baidujs-1.nonecase   
2. https://blog.csdn.net/naibozhuan3744/article/details/81103433   
3. https://www.cnblogs.com/pozhu15/p/12525102.html   
4. https://blog.csdn.net/a844651990/article/details/90489487   
5. https://www.cnblogs.com/chinono/p/13376546.html   

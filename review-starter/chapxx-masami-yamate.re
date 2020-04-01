= Alexa Gadgets Toolkitと組み合わせるスキル開発

//image[tag][]{
//}

//flushright{
著者：Masami Yamate（@<href>{https://twitter.com/aikawa_japan, @aikawa_japan}）
//}

== はじめに
2019年9月に発表されたAlexa Gadgets Toolkit。
まだAlexaが浸透しきれていない日本では、Alexa Gadgets Toolkitを用いたスキル開発の事例は多くありません。
今回は、そんなAlexa Gadgets Toolkitを用いたスキル開発をご紹介いたします。

===== 本章の対象読者@<br>{}

本章の対象読者は、次のような方です。  

 * Alexaスキルの開発経験がある開発者向け(ノンコーティングは除く)

===== 本章の前提知識@<br>{}

本章を読み進めるにあたり、次の基礎知識が必要です。  

 * Alexaスキル開発に求められる技術全般

===== 本章の必要環境@<br>{}

本章を読み進めるにあたり、次の環境が必要です。  

 * Node.js
 * Python3
 * Raspberry Pi

== Alexa Gadgets Toolkitとは
Alexa Gadgets Toolkitを紹介する公式ページには下記のように記載されています。

//quote{
Alexa Gadgets Toolkitを使用すると、ユーザー独自のAlexa Gadgetを開発できます。Alexa Gadgetは、Alexaに接続されたアクセサリで、Bluetooth経由で互換性のあるAmazon Echoデバイスとやり取りします。 - Amazon Alexa Gadgets Toolkitとは より引用
//}

そもそもAlexaGadgetとは、アメリカで発売されているEcho ButtonのようなAmazon Echoデバイスと接続し、利活用することができるEchoデバイスのアクセサリーのことを指します。
一言で言うと、Echoデバイスの機能を拡張する子機のようなものです。@<br>{}
そんなAlexa Gadgetを開発者が自由に開発するためのツールがAlexa Gadgets Toolkitです。

=== Alexa Gadgets Toolkitで、できること
Alexa Gadgets Toolkitを用いると大きく分けて2つのことが実行できます。

==== Alexaスキルからガジェットにアクセスする
カスタムスキルからガジェットに対してイベントを送付してLEDの色を変更したり、ガジェットのボタン操作でカスタムスキルから能動的な応答をさせたりすることができます。

==== Alexaネイティブの機能と連動してガジェットにイベントを送付する
Alexa標準のタイマーやアラームなどのイベントと紐付けてガジェットに対してイベントを送付することができます。利活用できる機能は下記の通りです。

 * スピーチマークデータ
 ** Alexaの読み上げデータをガジェット側で取得できます
 * 通知
 ** Amazonで買い物を行った商品の配送通知などがEchoデバイスに届いたタイミングで届く通知イベントをガジェット側でも利用することが可能になります。
 * タイマー、アラーム、リマインダー
 ** Echoデバイスで設定されたタイマー、アラーム、リマインダーの設定や解除のイベントをガジェット側で利用することが可能です。
 * ウェイクワード
 ** Echoデバイスに対して「Alexa、・・・・」と呼びかけたイベントをガジェット側で検知することが可能です。
 * 現在時刻
 ** Echoデバイスから現在時刻の取得が可能です。
 * Amazon Musicのテンポデータ
 ** Echoデバイスで再生中の楽曲のテンポデータの取得が可能です。

従来は、音声で入力し音声と画面(視覚情報)の2つの出力でユーザーに対してレスポンスしていましたが、
Alexa Gadgets Toolkitの登場により音声以外の入力方法と音声以外での出力方法を用いることが可能になったことが大きな変化です。
通常のAlexaのカスタムスキルでは、人と音声の対話で機能が完結することが多いです。Echo Showをはじめとした画面付きEchoデバイスでは、付随情報として視覚情報を
利用することができます。従来であれば、音声と視覚の2つの視点からカスタムスキルを開発していましたが、
Alexa Gadgets Toolkitを用いることで今までと異なる視点を持ったカスタムスキルの開発が、可能になったと考えています。@<br>{}
例えば、視覚障害を持つ方に向けるのであれば、カラーセンサーを搭載したAlexa gadgetと、カラーセンサーの入力値を読み上げるカスタムスキルをつくることで、
普段目にすることが難しい日常にあるモノの色情報を気軽に知ることができるようなスキルの開発も可能になります。
Amazon Alexaとしてはエンターテインメント分野に対してAlexa gadgetを今後積極的に利活用していきたい雰囲気もありますが、
発想と着眼点次第では今までと異なる層に対してもカスタムスキルを提供することが可能とも考えています。

=== Alexa Gadgets Toolkitの仕組み
Alexaと直接Alexa Gadgetが接続するのではなくEchoデバイスとAlexa GadgetがBluetooth経由で接続し、
Echoデバイスを介してAlexaプラットフォームで処理を行った後にスキル側にリクエストが届きます。
通常であれば「Alexa、hogehogeを開いて、fugafugaをして」という発話をAlexa側で解釈し、スキル側にリクエストを送付しています。
その発話が人ではなくガジェットからイベントを発火できるようになったというイメージだと分かりやすいかもしれません。
(厳密にはガジェットからのイベント発火は、カスタムスキルを呼び出し後からしか利用できません。)
よくスマートホームスキルと比較されることがありますが、スマートホームスキルの場合ガジェットがWi-Fiに直接接続する必要があり、
デバイスを管理するための独自のデバイスクラウドの開発・運用が必要でした。
また、デバイスクラウドとAlexaアカウントを紐付けるためのアカウントリンクの作業も必要であったため、
開発者目線では開発・運用コストが非常に高く、利用までの手順が多く煩わしさを感じることが多くなってしまいました。
一方Alexa Gadgetは、EchoデバイスとBluetoothで接続するだけで利活用できるため、
デバイスクラウドとアカウントリンクの作業も不要になり利用までの複雑な手順がなく、簡単に活用することができます。
また、Echoデバイスとガジェットが直接接続するため、デバイスクラウドの開発が不要で開発・運用コストも
スマートホームスキルに比べると格段に低くなっています。
さらに、この後の開発例で紹介するカスタムスキルAPIを用いることで、より複雑なスキルを開発することが可能です。

//embed[latex]{
\clearpage
//}

== ガジェットをセットアップする
 さて今回の本題です。実際にAlexa Gadgets Toolkitを用いてカスタムスキルの開発を行っていきましょう。
 この章の読者対象は、すでにカスタムスキルを自分自身で実装することができるような上級者向けのスキル開発者を想定しています。
 そのため、Raspberry PiのOSセットアップや環境構築部分に関しては割愛します。
 今回のスキル開発には下記の環境で行います。
 環境によって異なる点が生まれる場合もありますが、必要に応じて読み替えて対応してください。

 * Raspberry Pi
 ** Raspberry Pi 3 Model B
 * OS
 ** Raspbian 9.11

=== AmazonIDとシークレットキーの発行
Alexa Voice Serviceコンソール画面よりガジェットの登録を行い、ガジェットを識別するために利用するAmazonIDとシークレットキーを発行します。
コンソールから製品ページを開き、製品を作成するボタンをタップし、新規のAVS製品の登録を行ってください。
登録時に下記の項目の入力が必要です。

 * 製品名
 ** ガジェットの名称
 * 製品ID
 ** ガジェットを識別するためのID(何でもいいです)
 * 製品タイプ
 ** Alexa ガジェットの項目を選択してください
 * 商品カテゴリ
 ** 任意のカテゴリを選択しましょう
 * 製品概要
 ** 適当な説明文を入力しましょう
 * エンドユーザーは、商品とどのようにやり取りするのでしょうか?
 ** 今回はデモ用なので適当な項目を選択すれば大丈夫です
 * 画像をアップロード
 ** 今回は不要です
 * この製品を商品として配信する予定ですか ?
 ** デモ用なので「いいえ」を選択します
 * これは子供向け商品、それ以外は13歳以下の子供向けですか ?
 ** デモ用なので「いいえ」を選択します

 各項目入力後、「次へ」ボタンをタップし製品を登録します。
 製品登録後、@<b>{AmazonID}と@<b>{Alexa ガジェットのシークレットキー}が発行されます。
 ガジェットのセットアップ時に必要となるので記録しておきます。
 合わせて、ガジェットとガジェット対応のカスタムスキル開発時に@<b>{製品ID}も必要となりますので記録しておきましょう。コード上で、製品IDを用いるためキャメルケースにて製品IDを記述しましょう。

=== ガジェットにAmazonIDとシークレットキーを登録する
今回セットアップには、Alexa Gadgets Toolkitのサンプルコードに含まれているsetup.shを用いて行います。
Raspberry Piにsshでログインを行い、任意のディレクトリまで移動した後に下記コマンドを実行しサンプルコードをgit cloneしましょう。
//cmd{
$ git clone https://github.com/alexa/Alexa-Gadgets-Raspberry-Pi-Samples.git
//}
git clone後、サンプルコードのディレクトリまで移動し、launch.pyを実行します。
//cmd{
$ cd Alexa-Gadgets-Raspberry-Pi-Samples/
$ sudo python3 launch.py
//}

実行後、いくつかの質問項目で入力が求められますので、下記のように実行していきましょう。
実行中に事前に登録したAmazonIDとガジェットのシークレットキーの入力が求められます。

//cmd{
Do you want to configure all examples with your Alexa Gadget credentials (y/n)? 
$ y (Yesを選択)
Enter the Amazon ID for your gadget: 
$ (ガジェットのAmazon IDを入力)
Enter the Alexa Gadget Secret for your gadget: 
$ (ガジェットのシークレットキーを入力)

~ 色々走る ~

The Alexa Gadgets Raspberry Pi launch script provided herein will retrieve the 'Bluez-5.50' package at install-time from third-party sources. There are terms and conditions that you need to agree to abide by if you choose to install the 'Bluez-5.50' package (https://git.kernel.org/pub/scm/bluetooth/bluez.git/tree/COPYING?h=5.50). This script will also enable you to modify and install the 'bluez-5.50' package to enable notification callbacks after reconnections to a paired Echo device. This is required for communication between your gadget and the Echo device over BLE. If you do not agree with every term and condition associated with 'Bluez-5.50', enter 'QUIT', else enter 'AGREE'.
$ AGREE

~ 色々走る ~

Your gadget is currently configured to use BT transport mode.
Do you want to switch to BLE transport mode (y/n)?
$ n (今回BLEモードではないのでn)
+------------------------------+
|            SUCCESS           |
+------------------------------+
//}

Successと表示されたらRaspberry PiをRebootします。
//cmd{
$ sudo reboot
//}

=== Echoデバイスとガジェットの接続テストを行う
サンプルコードを用いて接続テストを行います。
サンプルコードを実行する前に、初期化ファイルにガジェットのAmazonIDとシークレットキーを追記します。
今回のテストでは、wakewordサンプルを利用しますのでwakeword.iniに追記しましょう。
//cmd{
$ cd ~/Alexa-Gadgets-Raspberry-Pi-Samples/src/examples/wakeword
$ vi wakeword.ini
//}

//cmd{
[GadgetSettings]
amazonId = [AmazonID]
alexaGadgetSecret = [シークレットキー]

[GadgetCapabilities]
Alexa.Gadget.StateListener = 1.0 - wakeword
//}

追記後、セーブを行い下記コマンドを実行し、サンプルを実行します。
//cmd{
$ python3 wakeword.py
INFO:agt.alexa_gadget:Now in pairing mode over BT. Pair Gadget HogeFuga in the Alexa App.
//}
実行後に「INFO:agt.alexa_gadget:Now in pairing mode over BT.」というメッセージが表示されれば起動に成功しています。
Alexaアプリを開き、デバイス→Echo・Alexa→ガジェットをペアリングするEchoデバイス名→Alexa Gadgetをペアリングの順にタップしましょう。
使用可能なデバイスの一覧にコマンドラインに表示されている「Gadget HogeHoge」と一致するデバイスを選択します。
選択後、〇〇と接続しましたというアラートが表示されれば接続成功です。

//cmd{
$ INFO:agt.alexa_gadget:Connected to Echo device with address xx:xx:xx:xx:xx:xx over BT
//}
Echoデバイスとの接続後、コマンドライン上に接続したEchoデバイスのアドレスが表示され、ガジェット側は待機状態になります。
wakewordのサンプルコードなので、Echoデバイスに対して「Alexa」と呼びかけると、コマンドライン上にwakewordの状態が表示されます。
//cmd{
INFO:__main__:Wake word active - turn on LED
INFO:__main__:Wake word cleared - turn off LED
//}

Alexaと呼びかけるとturn on LEDとログに表示され、キャンセルと呼びかけるとturn off LEDが表示されます。

== カスタムスキルを開発する
サンプルの実行まで、できたら実際にカスタムスキルの開発を行っていきましょう。
まず、Alexa Developer Consoleから新規のスキルを作成します。
スキル名などはデモ用であることが分かる名称にしましょう。@<br>{}
スキル作成後、インターフェイスの設定項目内に含まれるカスタムインターフェイスコントローラーをONにします。
ONにすることで、カスタムスキルでAlexaガジェットからの入力やAlexaガジェットへの出力に対応することが可能になります。
しかし、カスタムインターフェイスを有効化しただけではAlexaガジェットからのイベントを取得することはできません。
Alexaガジェットは、クラウドに接続されておらずEchoデバイスと接続されているため、カスタムスキルからEchoデバイスに対して
「今からガジェットのイベントの監視を開始」するということを明示的にリクエストしなければなりません。@<br>{}
そのため、Alexaガジェットを用いたカスタムスキルをつくる際、冒頭からAlexaガジェットに接続されていないと実現できないようなシナリオはできません。
なので、ユーザーが意図的にスキルを呼び出し、カスタムスキルが立ち上がった後にスキル内の体験を拡張するための手段としてガジェットをつくる必要があります。

=== ガジェットを利用する
通常のカスタムスキルであれば、ユーザーIDをもとにセーブデータの有無を判定し、前回の続きからスキルを再開するようなフローが想定できます。
ガジェットを用いる場合は、Echoデバイスに接続されているガジェットの有無の確認、ガジェットから送付されるイベントの監視を開始するなどのステップが必要になります。

//embed[latex]{
\clearpage
//}

//image[flowchart-gadget-request][ガジェット利用時のフローチャート図]{
//}

では、それぞれのステップで行う内容について見ていきましょう。

==== スキルを起動する
本項目は、通常のカスタムスキルを開発する際も共通ではありますが、「Alexa、hogehogeを開いて」とスキルを立ち上げた際にリクエストされる
LaunchRequestをどう処理するかという部分になります。
ガジェットとの接続を必要とするカスタムスキルであれば、LaunchRequest時に利用可能なガジェットの取得、検証、登録を行うことで
Echoデバイスとガジェットが接続されていない場合、ユーザーにガジェットとの接続を促すことが可能になります。
また、特定のカスタムインテントでも起動することができるので、特定のカスタムインテントからの起動の場合のみガジェットを利用するような設計も可能です。
//list[template][今回のスキルの雛形][javascript]{
const Alexa = require('ask-sdk-core');

let skill;
exports.handler = function(event, context) {
    if (!skill) {
        skill = Alexa.SkillBuilders.custom()
            .addRequestHandlers(
                handler.LaunchRequestHandler
            )
            .addErrorHandlers(handler.ErrorHandler)
            .create();
    }
    return skill.invoke(event, context);
};

// MARK: Handlers
const handler = {
    LaunchRequestHandler: {
        canHandle(handlerInput) {
            return checkIntentTypeName(handlerInput, 'LaunchRequest');
        },
        async handle(handlerInput) {
            return handlerInput.responseBuilder
                .speak("ガジェットのスキルを立ち上げました！")
                .withShouldEndSession(true)
                .getResponse();
        }
    },
    HelpRequestHandler: {
        canHandle(handlerInput) {
            return checkIntentTypeName(handlerInput, 'IntentRequest', 'AMAZON.HelpIntent');
        },
        async handle(handlerInput) {
            return handlerInput.responseBuilder
                .speak("使い方です！")
                .withShouldEndSession(true)
                .getResponse();
        }
    },
    DefaultHandler: {
        canHandle() {
            return true;
        },
        handle(handlerInput) {
            return handlerInput.responseBuilder
                .getResponse();
        }
    },
    ErrorHandler: {
        canHandle() {
            return true;
        },
        handle(handlerInput) {
            return handlerInput.responseBuilder
                .speak("申し訳ありません、内部エラーが発生しました。")
                .withShouldEndSession(true)
                .getResponse();
        },
    }
}

// MARK:　Utility methods
/**
 * インテントタイプ、インテント名が一致しているか確認し返却します
 *
 * @param {*} handlerInput
 * @param {*} typeName
 * @param {*} intentName
 * @returns
 */
function checkIntentTypeName(handlerInput, typeName, intentName) { 
    let request = handlerInput.requestEnvelope.request;
    let isMatch = false;
    //インテントタイプのチェックを行う
    if (typeName && request.type === typeName) {
        isMatch = true;
    } else {
        isMatch = false;
    }
    //リクエストインテント名のチェックを行う
    if (intentName) {
        if (request.intent && request.intent.name === intentName) {
            isMatch = true;
        } else { 
            isMatch = false;
        }
    }
    return isMatch;
}
//}
今回は、上記のカスタムスキルのコードに適宜Gadgetと連携する場合の処理を追加していきます。
現時点では、カスタムスキルを開いた直後に「ガジェットのスキルを立ち上げました！」と発話するだけの内容です。

//embed[latex]{
\clearpage
//}

=== 利用可能なガジェットを取得、検証、エンドポイントの登録を行う
開発者コンソールからカスタムインターフェイスコントローラーをONにしていても、スキル側から直接ガジェットのエンドポイントを取得することはできません。
実際に利用可能なガジェットの取得を行うには、Endpoint Enumeration APIを呼び出すことが必要になります。
Endpoint Enumeration APIのリクエストには、「apiEndpoint」と「apiAccessToken」の2つのクエリが必要になるので、
Alexaのリクエストに含まれるコンテキストオブジェクトの「apiEndpoint」と「apiAccessToken」を取得し、Endpoint Enumeration APIを呼び出し、
利用可能なガジェットのエンドポイントの一覧を取得します。
取得したエンドポイントは、カスタムスキルのセッションが継続している間は有効なので取得後セッション情報に登録し、以後はセッション情報に登録されているエンドポイントを
利用することが推奨されています。

また、Endpoint Enumeration APIで取得できるエンドポイントのガジェットの要件は下記の通りです。

 * ガジェットがEchoデバイスに接続されていること
 * ガジェットとスキルが同じ開発者アカウントで開発者ポータルに登録されていること

ガジェットとスキルが同じ開発者アカウントで開発者ポータルに登録されている必要あるため、ガジェットのスクリプトのみオープンソースで公開し、
カスタムスキルのみストアで公開することはできません。そのため、ストアで公開するスキルの場合は、ガジェットも何らかの方法で配布、販売する必要があります。

//list[endpoints][エンドポイントの取得][javascript]{
/**
  *　接続されているAlexaガジェットのエンドポイントを取得する
 *
 * @param {*} handlerInput
 * @returns EndPointの配列 [String]
 */
function gadgetEndPoints (handlerInput) {
    let { context } = handlerInput.requestEnvelope;
    let { apiEndpoint, apiAccessToken } = context.System;

    apiEndpoint = (apiEndpoint || '').replace('https://', '');

    return new Promise(((resolve, reject) => {
        var options = {
            host: apiEndpoint,
            path: '/v1/endpoints',
            method: 'GET',
            headers: {
                'Content-Type': 'application/json',
                'Authorization': 'Bearer ' + apiAccessToken
            }
        };

        const request = Https.request(options, (response) => {
            response.setEncoding('utf8');
            let returnData = '';

            response.on('data', (chunk) => {
                returnData += chunk;
            });

            response.on('end', () => {
                let result = JSON.parse(returnData);
                let endPoints = result.endpoints || [];
                resolve(endPoints);
            });

            response.on('error', (error) => {
                reject(error);
            });
        });
        request.end();
    }));
}
//}

上記のメソッドは、Alexaのリクエスト情報を引数として渡すとEndpoint Enumeration APIをリクエストし、利用可能なエンドポイントを配列として返却する処理を担っています。
利用可能なデバイスが存在しない場合は、返却されるエンドポイントの配列が空配列となるので、配列の要素数が0であるときはガジェットとの接続ができていない旨をAlexaに発話させて、ユーザーにガジェットとの接続を促すようなフローにすると良いでしょう。

//list[endpoints-save][エンドポイントの登録][javascript]{
/**
 * 接続されているガジェットのEndPointをsessionAttributeに登録する
 *
 * @param {*} handlerInput
 * @param {[String]} endPointIds
 */
function setEndPoints(handlerInput, endPointIds) {
    if (!endPointIds) {
        return;
    }
    const attributesManager = handlerInput.attributesManager;
    let sessionAttributes = attributesManager.getSessionAttributes();
    sessionAttributes.endPointIds = endPointIds;
    attributesManager.setSessionAttributes(sessionAttributes);
}
//}

取得したエンドポイントは、sessionAttributesに登録します。
以後はセッションが継続している間は、sessionAttributesに登録したendpointを利用することが可能です。
以上のガジェットのエンドポイント取得に関連するコードを追加すると次のようになります。

//embed[latex]{
\clearpage
//}

//list[template-add-endpoints][エンドポイント取得部分追加後][javascript]{
const Alexa = require('ask-sdk-core');
const Https = require('https');　// 追加
const uuid = require('uuid/v4'); // 追加

// MARK: Handlers
const handler = {
    LaunchRequestHandler: {
        canHandle(handlerInput) {
            return checkIntentTypeName(handlerInput, 'LaunchRequest');
        },
        async handle(handlerInput) {
            // 以下、本項目での追加箇所

            // ガジェットと接続が確認できた場合
            // 接続されているEndpoint一覧を取得します
            let endPoints = await gadgetEndPoints(handlerInput);
            if (0 < endPoints.length) {
                setEndPoints(endPoints);
                setSessionToken(handlerInput);
                return handlerInput.responseBuilder
                    .speak("ガジェットと接続できました！")
                    .getResponse();
            }else{
                return handlerInput.responseBuilder
                    .speak("ガジェットと接続されていないみたいです")
                    .withShouldEndSession(true)
                    .getResponse();
            }
        }
    }
}

// MARK:　Utility methods
/**
 * インテントタイプ、インテント名が一致しているか確認し返却します
 *
 * @param {*} handlerInput
 * @param {*} typeName
 * @param {*} intentName
 * @returns
 */
function checkIntentTypeName(handlerInput, typeName, intentName) { 
    let request = handlerInput.requestEnvelope.request;
    let isMatch = false;
    //インテントタイプのチェックを行う
    if (typeName && request.type === typeName) {
        isMatch = true;
    } else {
        isMatch = false;
    }
    //リクエストインテント名のチェックを行う
    if (intentName) {
        if (request.intent && request.intent.name === intentName) {
            isMatch = true;
        } else { 
            isMatch = false;
        }
    }
    return isMatch;
}

// MARK: Gadget methods
/**
  *　接続されているAlexaガジェットのエンドポイントを取得する
 *
 * @param {*} handlerInput
 * @returns EndPointの配列 [String]
 */
function gadgetEndPoints (handlerInput) {
    let { context } = handlerInput.requestEnvelope;
    let { apiEndpoint, apiAccessToken } = context.System;

    apiEndpoint = (apiEndpoint || '').replace('https://', '');

    return new Promise(((resolve, reject) => {
        var options = {
            host: apiEndpoint,
            path: '/v1/endpoints',
            method: 'GET',
            headers: {
                'Content-Type': 'application/json',
                'Authorization': 'Bearer ' + apiAccessToken
            }
        };

        const request = Https.request(options, (response) => {
            response.setEncoding('utf8');
            let returnData = '';

            response.on('data', (chunk) => {
                returnData += chunk;
            });

            response.on('end', () => {
                let result = JSON.parse(returnData);
                let endPoints = result.endpoints || [];
                resolve(endPoints);
            });

            response.on('error', (error) => {
                reject(error);
            });
        });
        request.end();
    }));
}

/**
 * sessionAttributeに登録されているセッショントークンを取得する
 *
 * @param {*} handlerInput
 * @returns 登録されているToken
 */
function sessionToken (handlerInput) {
    const attributesManager = handlerInput.attributesManager;
    let sessionAttributes = attributesManager.getSessionAttributes();
    if (sessionAttributes.token) {
        return sessionAttributes.token;
    }
    return null;
}

/**
 * sessionTokenを新規に生成し、sesstionAttributeに登録する
 *
 * @param {*} handlerInput
 */
function setSessionToken (handlerInput) {
    const attributesManager = handlerInput.attributesManager;
    let sessionAttributes = attributesManager.getSessionAttributes();
    sessionAttributes.token = uuid();
    attributesManager.setSessionAttributes(sessionAttributes);
}

/**
 * 接続されているガジェットのEndPointをsessionAttributeに登録する
 *
 * @param {*} handlerInput
 * @param {[String]} endPointIds
 */
function setEndPoints(handlerInput, endPointIds) {
    if (!endPointIds) {
        return;
    }
    const attributesManager = handlerInput.attributesManager;
    let sessionAttributes = attributesManager.getSessionAttributes();
    sessionAttributes.endPointIds = endPointIds;
    attributesManager.setSessionAttributes(sessionAttributes);
}

//}

//embed[latex]{
\clearpage
//}

一部省略箇所はありますが、LaunchRequestでの呼び出しは以上です。
スキル起動後、ガジェットのエンドポイント一覧を取得しエンドポイントの件数が0件でなければ「ガジェットと接続できました！」と発話させ、そうでない場合は、「ガジェットと接続されていないみたいです」と発話させています。
ここで注意点として、ガジェットと接続できたことを確認後ガジェットのイベントを扱う場合は、@<b>{.withShouldEndSession(true)}として明示的にセッションクローズしてはいけません。
明示的にセッションクローズを行ってしまうと、スキルが閉じてしまいガジェットとのやり取りに移ることができなくなるので注意が必要です。

=== StartEventHandlerディレクティブを返却する
CustomInterfaceController.StartEventHandlerをタイプとするディレクティブを生成し、Alexaの応答内容をレスポンスとして返却することで、
ガジェットのイベント監視が開始されます。

//list[StartEventHandler][StartEventHandlerディレクティブの生成][javascript]{
/**
 * ガジェットからのEventを監視するdirectiveを生成する
 *
 * @param {*} names
 * @param {*} nameSpaces
 * @param {*} token
 * @param {*} filterMatchAction
 * @param {*} durationMs
 * @param {*} expirationPayload
 * @returns
 */
function createStartEventHandlerDirective(names,
                                          nameSpaces,
                                          token,
                                          filterMatchAction,
                                          durationMs,
                                          expirationPayload) {
    expirationPayload = expirationPayload || {};
    durationMs = durationMs || 90000;
    console.log(names);
    let nameFilters = [];
    for (let i in names) {
        let name = names[i];
        nameFilters.push({ '==': [{ 'var': 'header.name' }, name]});
    }
    let nameSpaceFilters = [];
    for (let i in nameSpaces) {
        let nameSpace = nameSpaces[i];
        nameSpaceFilters.push({ '==': [{ 'var': 'header.namespace' }, nameSpace] })
    }
    let filters = nameFilters.concat(nameSpaceFilters);
    let filterExpression = {
        'and': filters
    };
    return {
        type: "CustomInterfaceController.StartEventHandler",
        token: token,
        eventFilter: {
            filterExpression: filterExpression,
            filterMatchAction: filterMatchAction
        },
        expiration: {
            durationInMilliseconds: durationMs,
            expirationPayload: expirationPayload
        }
    }
}
//}

前述のメソッドは、CustomInterfaceController.StartEventHandlerディレクティブを生成するためのものです。
CustomInterfaceController.StartEventHandlerには下記の項目を含めることが可能です。

 * type
 ** CustomInterfaceController.StartEventHandlerを設定する
 * token
 ** 新しいリクエストと以前のリクエストを識別するための一意なID。Alexaのリクエストに含まれるrequestIdを用いることが推奨されています。
 * expiration.durationInMilliseconds
 ** イベントハンドラーの有効期限(ミリ秒単位)最大で90秒まで有効期限を設定することが可能です。
 * expiration.expirationPayload
  ** イベントハンドラーの有効期限が切れた際に、スキル側に送付される内容を定義するオブジェクトです。基本的にはEventsReceivedリクエスト内で固定の内容を返却することが可能なので、あまり利用しないです。利用開始時のなんらかの情報をタイムアウト時に返却する場合などに利用します。
 * filterExpression
 ** イベントを実行する名前空間の判定式を含めます。上記のメソッドでは、実行したい名前空間のみ指定するだけで判定式を自動で生成することができます。
 * filterMatchAction
 ** SEND または SEND_AND_TERMINATEを設定します。SENDを指定した場合は、フィルターと一致したすべてのカスタムイベントをスキルに送付し、SEND_AND_TERMINATEは、フィルターと一致したカスタムイベントをスキルに送付し、イベントハンドラーを終了します。

filterExpressionの組み立てには、名前空間とガジェットで利用したいイベント名が必要です。
名前空間は、Alexa Voice Service コンソール画面で登録した製品IDを利用します。「Custom + 【製品ID】」を指定します。
イベント名は、任意の名称をキャメルケースで記入してください。今回Alexaガジェット側のスクリプトは、Pythonにて記述します。
サンプルコードで提供されているagtパッケージは、送付したイベント名を自動でスネークケースに変換してPython側のメソッド名を実行するため、
Node.jsで記述するカスタムスキル側はキャメルケース、ガジェット側はスネークケースを必ず守る必要があります。
守れていない場合、意図しない動きになりますので注意が必要です。

注意点としてCustomInterfaceController.StartEventHandlerディレクティブを用いたガジェットのイベント監視は、カスタムスキルの中で1つだけ有効になります。
そのため、複数のイベントを同時に監視することはできません。下記のレスポンスの場合どうなるか見てみましょう。

//list[StartEventHandlerResponse][StartEventHandlerディレクティブの返却][javascript]{
handlerInput.responseBuilder
    .speak("ガジェットのスキルを立ち上げました！")
    .addDirective(sessionStartDirective) // 送付するディレクティブ
    .addDirective(monitoringFirstDirective) // 監視するためのディレクティブ1
    .addDirective(monitoringSecondaryDirective) // 監視するためのディレクティブ2
    .getResponse();
//}

上記のレスポンスは、カスタムスキルからガジェットにイベントを送付するディレクティブとガジェットから送付される2つのイベントを監視しようとCustomInterfaceController.StartEventHandlerを含むディレクティブを付与しています。
前述の通り、CustomInterfaceController.StartEventHandlerディレクティブによるガジェットのイベント監視は1つしか含めることができません。
そのため、「monitoringFirstDirective」は後から追加した「monitoringSecondaryDirective」に上書きされてしまい、「monitoringFirstDirective」のイベントは監視できなくなってしまいます。
ガジェットからの入力が複数パターンあり、ガジェットの入力値によってスキルのシナリオが変わるような設計の場合は、ガジェットから送付されるイベントは1つだけにして、
イベントに含まれるPayloadにイベントフラグなどを含め、スキルの処理を変更するような実装が良いと考えています。

CustomInterfaceController.StartEventHandlerディレクティブでは、ガジェットからのイベントを監視することが可能でした。
逆にカスタムスキルからガジェットにイベントを送付する場合には、CustomInterfaceController.SendDirectiveを利用します。

//embed[latex]{
\clearpage
//}

//list[SendDirective][SendDirective][javascript]{
/**
 * ガジェットに送付するdirectiveを生成します
 *
 * @param {*} name
 * @param {*} namespace
 * @param {*} endPointId
 * @param {*} payload
 * @returns 送付するDerective
 */
function createSendDirective(name, namespace, endPointId, payload) {
    payload = payload || {};
    return {
        type: 'CustomInterfaceController.SendDirective',
        header: {
            name: name,
            namespace: namespace
        },
        endpoint: {
            endpointId: endPointId
        },
        payload: payload
    }
}
//}

上記メソッドは、CustomInterfaceController.SendDirectiveディレクティブを生成するためのものです。
CustomInterfaceController.SendDirectiveには下記の項目を含めることが可能です。

 * type
 ** CustomInterfaceController.SendDirectiveを設定する
 * endpointId
 ** ディレクティブ送付先のガジェットのエンドポイントのID
 * namespace
 ** ガジェットの製品ID (Custom. + 【製品ID】)
 * name
 ** 開発者が定義したディレクティブの名称。ガジェット側でのメソッド名にもなります
 * payload
 ** ガジェットに送付したい自由形式のJSONオブジェクト

CustomInterfaceController.SendDirectiveは、CustomInterfaceController.StartEventHandlerディレクティブと異なり、
1つのレスポンスに複数付与することが可能です。
また、1つのCustomInterfaceController.SendDirectiveで1つのガジェットへのイベント送付になるため、
複数のガジェットに対してイベントを送付する場合は送付したいガジェットの数分のCustomInterfaceController.SendDirectiveを付与する必要があります。
ここまでの追加内容をLaunchRequestHandler内の処理に追加すると以下のようになります。

//list[template-add-sendDirective][StartEventHandler・SendDirective追加後][javascript]{
const handler = {
    LaunchRequestHandler: {
        canHandle(handlerInput) {
            return checkIntentTypeName(handlerInput, 'LaunchRequest');
        },
        async handle(handlerInput) {
            // 接続されているEndpoint一覧を取得します
            let endPoints = await gadgetEndPoints(handlerInput);
            if (0 < endPoints.length) {
                setEndPoints(endPoints);
                setSessionToken(handlerInput);
                // セッショントークンの取得
                let sessionToken = await gadgetUtil.sessionToken(handlerInput);
                // 今回は接続されているEndPointのうち0番目のgadgetを利用する
                let sendEndPoint = endPoints[0];
                let sendEndPointId = sendEndPoint.endpointId;

                // CustomInterfaceController.SendDirectiveの生成
                let name = 'launch';
                let nameSpace = 'Custom.gadget'
                // ガジェットからのイベントで発話させる内容をpayloadに含める
                let sendPayload = {
                    response: "この発話はガジェットからです。"
                };
                let sendDirective = createSendDirective(name,
                     nameSpace, 
                     sendEndPointId, 
                     sendPayload);
                // CustomInterfaceController.StartEventHandlerの生成
                let names = ['SkillHandler'];
                let nameSpaces = [nameSpace];
                let startEventHandler = createStartEventHandlerDirective(names,
                     nameSpaces,
                     sessionToken, 
                     "SEND_AND_TERMINATE", 
                     null, 
                     null);
                return handlerInput.responseBuilder
                    .speak("ガジェットと接続できました！")
                    .addDirective(sendDirective)
                    .addDirective(startEventHandler)
                    .getResponse();
            }else{
                return handlerInput.responseBuilder
                    .speak("ガジェットと接続されていないみたいです")
                    .withShouldEndSession(true)
                    .getResponse();
            }
        }
    }
}
//}

今回は、カスタムスキル起動後にガジェットに対して@<b>{start}というイベントを送付します。
この際に送付するPayloadにはガジェットからのイベントをカスタムスキル側で受け取った際の発話内容を含めておきます。
ガジェット側は@<b>{start}というイベントを受け取り、オウム返しするようにPayloadに含めた発話内容をカスタムスキル側に再度戻し、
カスタムスキルでガジェットからのイベントを捕捉した際に、戻ってきたPayloadに含まれる発話内容を読み上げる流れになります。

=== ガジェット側でスキルから送付されたイベントを処理する
前の項目までは、カスタムスキルの返却値にガジェットに対するイベントのディレクティブを含めるということまで行いました。
ここからは、ガジェット側のスクリプトの準備を行います。

まず、実行するスクリプトファイルとiniファイルを生成します。
ここでは、gadget.pyとgadget.iniとしましょう。
gadget.iniは下記のように記載します。

//list[gadget.ini][gadget.ini]{
[GadgetSettings]
amazonId = ここにID
alexaGadgetSecret = ここにシークレットキー

[GadgetCapabilities]
Custom.gadget = 1.0
//}

GadgetSettings以下にAlexa Voice Serviceコンソール画面で生成した、Alexa GadgetのAmazonIDとシークレットキーを記載します。
GadgetCapabilities以下には、「Custom. + 【製品ID】= 1.0」と記載します。3つの記載ができればiniファイルの実装は完了です。

次にGadget側のスクリプトの準備を行います。

//list[gadget.py][gadget.py][python]{
import json
import logging
import sys
import threading
import signal
import time

from agt import AlexaGadget
from datetime import datetime

logging.basicConfig(stream=sys.stdout, level=logging.INFO)
logger = logging.getLogger(__name__)

class gadget(AlexaGadget):
    def __init__(self):
        super().__init__()

    def on_custom_sadget_start(self, directive):
        # 送付されたPayloadを読み込む
        payload = json.loads(directive.payload.decode("utf-8"))
        # オウム返しで、Gadgetからカスタムスキルに受け取ったPayloadを返却する
        self.send_custom_event(
                'Custom.gadget', 'SkillHandler', payload)

if __name__ == '__main__':
    try:
        gadget().main()
    finally:
        logger.info("finish")
//}

カスタムスキルから送付されたイベントは、上記のコードのメソッドが呼び出されます。
各イベントの実行するメソッド名には命名規則があり、@<b>{on_custom_}+@<b>{【製品ID】_}+@<b>{イベント名}のような命名規則を守り定義する必要があります。
準拠しないメソッドは、呼び出されないためカスタムスキルからのイベントをハンドリングすることはできません。
上記のコードでは、startというイベントが発火後pyloadデータを取得し、agtパッケージに定義されている@<b>{send_custom_event}を呼び出し、
受け取ったPayloadオブジェクトをカスタムスキルにオウム返ししています。
今回は、オウム返ししてAlexaに能動的な発話をさせるまでしか行いませんが、
実際には受け取ったイベントを元にRaspberry Piに接続されたセンサーやボタンの入力値を元にカスタムスキル側にイベントを返却することでAlexa Gadgetならではのスキル開発が可能です。

=== ガジェットから送付されたイベントを処理する
ガジェットから送付されたイベントは、CustomInterfaceController.StartEventHandlerで設定したイベントのフィルター条件を満たす場合に、
CustomInterfaceController.EventsReceivedとしてリクエストを受け取ります。

//list[CustomInterfaceController.EventsReceived][CustomInterfaceController.EventsReceived][javascript]{
CustomInterfaceEventHandler: {
    canHandle(handlerInput) {
        return checkIntentTypeName(handlerInput,
            'CustomInterfaceController.EventsReceived');
    },
    handle(handlerInput) {
        return handlerInput.responseBuilder
            .speak("イベントを受け取りました！")
            .getResponse();
    }
}
//}

CustomInterfaceEventHandlerを追加し、CustomInterfaceController.EventsReceivedの場合のレスポンスを生成します。
上記の状態では、監視しているイベントのフィルターとマッチしたイベントが飛んできたときにAlexaが「イベントを受け取りました！」と発話します。
ガジェットから送付されるデータは、Payloadオブジェクトに格納されていて、CustomInterfaceController.EventsReceivedの場合リクエストの中に含まれています。
どんなデータがガジェットから返却されたか下記のメソッドを用いてPayloadオブジェクトを抽出します。

//list[export-payload][Payloadの抽出][javascript]{
/**
 * ガジェットからのResponseに含まれるPayloadを取得する
 *
 * @param {*} handlerInput
 * @param {*} eventIndex
 */
function getPayload (handlerInput, eventIndex) {
    eventIndex = eventIndex | 0;
    let { request } = handlerInput.requestEnvelope;
    if (eventIndex < request.events.length) {
        let customEvent = request.events[eventIndex];
        return customEvent.payload;
    }
    return null;
}
//}

引数にEvent indexを用意していますが、スキルがガジェットからのイベントを複数受信する場合があります。
複数受信したイベントの中からどのイベントを処理するか判別する必要が発生します。
基本的には、ガジェットから同時にイベントが複数送付されるような設計にはならないので、最初に送られたイベントを利用して問題ありません。
複数のイベントをガジェット側からカスタムスキル側に送付する場合は、ガジェット側で送付するPayloadオブジェクトにシーケンスIDを保持させ、
カスタムスキル側で受信したPayloadオブジェクト内のシーケンスIDを元にイベントを処理することが推奨されています。
Payloadを抽出することはできたので、ガジェットからオウム返しされた発話内容をAlexaに発話させましょう。


//list[CustomInterfaceController.EventsReceived-speak-alexa][Payloadの内容を元に発話させる][javascript]{
CustomInterfaceEventHandler: {
    canHandle(handlerInput) {
        return checkIntentTypeName(handlerInput,
            'CustomInterfaceController.EventsReceived');
    },
    handle(handlerInput) {
        let payload = getPayload(handlerInput);
        let response = payload.response | "うまく受け取れなかったみたいです";
        return handlerInput.responseBuilder
            .speak(response)
            .getResponse();
    }
}
//}

今回は、送付したPayloadと同じものが返却されているので、responseに入っている文字列をそのままAlexaに発話させています。
payload.responseが取得できなかった場合は、「うまく受け取れなかったみたいです」と発話します。
ここまでは正常時のガジェットからのイベントを受け取った処理でした。
今回のサンプルでは、ガジェット側で受け取ったPayloadをオウム返ししているのでありえませんが、
イベントハンドラーの有効期限切れのときのリクエストも対応する必要があります。

//embed[latex]{
\clearpage
//}

有効期限切れの際は、カスタムスキルにCustomInterfaceController.Expiredが送付されます。下記のハンドラーを追加しましょう。

//list[CustomInterfaceController.Expired][CustomInterfaceController.Expired][javascript]{
CustomInterfaceExpiredHandler: {
    canHandle(handlerInput) {
        return checkIntentTypeName(handlerInput,
            'CustomInterfaceController.Expired');
    },
    handle(handlerInput) {
        return handlerInput.responseBuilder
            .speak("ハンドリングの有効期限が終わりました。")
            .getResponse();
    }
}
//}

イベントのハンドリング有効期限は最大で90秒です。90秒の間にガジェットからカスタムスキルへイベントが送られなかった場合、
自動的にCustomInterfaceController.Expiredが送付されセッションが閉じられます。

== おわりに
駆け足になりましたが、Alexa Gadgets Toolkitを組み合わせたスキル開発を簡単にご紹介させていただきました。
今回は、実際にセンサー類やLED、ボタンなどと紐付けていないのでガジェット感があまり感じられない内容でしたが、
Alexa Gadgets ToolkitはVUIの可能性・体験をより広げていくキッカケになると感じています。
プライベーターですとストア公開スキルの開発が難しい現状がありますが、ぜひ休日Hackの1つの選択肢として選ぶキッカケになれば嬉しいです。

また、下記のリポジトリで、Alexa Gadgets Toolkitのかゆいところを代行してくれるモジュールを開発しています。
もしよかったらご活用いただけますと幸いです。 @<br>{}
katana-alexa-gadget-tool-util - MasamiYamate@<br>{}
@<href>{https://github.com/MasamiYamate/katana-alexa-gadget-tool-util}


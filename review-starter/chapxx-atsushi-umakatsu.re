
= VUI開発におけるテストのおはなし

//image[tag][]{
//}

//flushright{
著者：horsewin（@<href>{https://twitter.com/HorseVictory/,@HorseVictory}）
//}


== はじめに


はじめまして。システム構築会社で勤めています、horsewin（@HorseVictory）です。本職では顧客への新規事業提案や業務課題解決に加えて、バリバリのフロント＆サーバサイドエンジニアとして働いています。VUIについてはAlexaが日本にローンチされる前から携わっており、金融向けのVUIアプリケーションなどを構築していました。@<br>{}
本章ではVUI、特にAlexaのテストに関するお話をします。今回取り組むモチベーションの話へ入る前に皆さんへ問いたいのですが、テストは好きでしょうか。私はあまり好きでありません（笑）。しかし、一般ユーザーに使っていただくアプリケーションを構築するプロにとって、テストは非常に重要で軽視できない存在です。バックエンド処理やWebアプリケーションのテスト手法は確立されている今日ですが、VUIのテスト手法が語られていることは少なく感じます（少なくともエンタープライズ領域において）。@<br>{}
そこで、本章では自身の知識整理も含めてVUIのテストの話をしようと考えた次第です。



===== 本章の対象読者@<br>{}


本章の対象読者は、次のような方です。  

 * Alexaのテストについて一段深く考えたい方
 * 公式ドキュメントに書いていないテストについて知りたい方
 * Alexaの開発が好きな方
 * VUIの開発が好きな方


===== 本章の前提知識@<br>{}


本章を読み進めるにあたり、次の基礎知識が必要です。  

 * スマートスピーカー、VUIを触ったことがある
 * VUIアプリケーションを開発したことがある
 * JSONやYAMLを読めること



//clearpage


== テストとは


Google先生に尋ねたところ、次のような回答を得ることができます。


//quote{
開発したシステムやモジュールなどが正しく動作するか、仕様書どおりに作成されているか、性能に問題はないかなど、システムやモジュールに対して完成度を確かめるために行う作業のこと
//}


VUIアプリケーションの文脈に沿うと、VUIアプリケーションが自身の達成したい、または提供したい目的どおりに動作するかとなります。ユーザーへのアプリケーション説明文章にかかれていることと異なる動作をしては問題ですよね。それを可能な限り防ごうという目的です。


=== テストの種類


テストと一言でいっても複数の種類や段階に分けたテストがあります。たとえば、システム開発の現場においては次に示すテストが段階的に実施されます。


==== 単体テスト（UT、ユニットテスト）


モジュール単体が正しく動作するかを確認するテストです。プログラムのメソッド単位のテストをイメージしてください。


==== 結合テスト（IT、インテグレーションテスト）


モジュールどうしを結合したテストです。エンドツーエンドテストともいわれます。声によるインプットを元にしてAlexaから想定とおりの応答を受け取れたかどうかをイメージするとわかりやすいです。


==== 総合テスト（ST、シナリオテスト）


すべてのモジュールを結合して、一連のシナリオを確認するテストです。事業会社のアプリケーションではQAテスト（品質保証テスト）と呼ばれているものです。１回の会話のやりとりだけでなく、一連のやりとり（たとえばレストランの注文）が問題なく実施できるかをイメージするとわかりやすいです。


==== ユーザー受入テスト（UAT、運用テスト）


受託アプリケーションにおいて実施されるテストです。事業会社のアプリケーションではステージングテストと呼ばれていると推察します。本番のリハーサルのようなテストを実施し、問題がないか確認するテストです。



//clearpage

VUIアプリケーションと言っても、上述したようなテストを実施することでテストの目的（VUIアプリケーションが自身の達成したい、または提供したい目的どおりに動作するか）を実現可能です。しかし、VUIアプリケーションの特有の検証項目が存在するのも事実です。次の節ではテストによるVUIアプリケーションの検証にディープダイブします。以降は各テストを指すときは、一般的に使われている呼び名である、括弧書きで記載したテスト名（ユニットテスト、インテグレーションテスト）で記載します。


== テストによるVUIの検証

=== 今回扱わないテスト


本来、VUIアプリケーションでは「会話シナリオの良さ」と「会話の質の良さ」の検証がもっとも難しく重要といわれています。@<br>{}
しかし、今回は「会話シナリオの良さ」と「会話の質の良さ」を向上させるテストは対象としません。設計したVUIアプリケーションの会話の正しさを検証するテストを対象とします。


=== レスポンスJSON


VUIアプリケーションでは、VUIアプリケーションを提供するプラットフォーム@<fn>{1}へ応答となるJSONファイルを渡すことで音声応答を生成します。VUIアプリケーションにインプットするデータもJSONです。あるインプットのJSONファイルから、応答の元となるJSONファイルが想定どおりに生成されるならば、そのモジュールは想定どおりに機能しているといえます。この検証は一般的な開発のユニットテストと同等レベルの検証です。@<br>{}
この検証はまさに通常のGUIアプリケーションのユニットテストに相当します。GUIアプリケーションの場合であっても、このレベルのテストのチェックはツールを使うことが多いです。レスポンスJSONをどのようにツールで検証するかは後述します。


//listnum[response.json][response.json][json]{
{
    "body": {
        "version": "1.0",
        "response": {
            "outputSpeech": {
                "type": "SSML",
                "ssml": "<speak>技術書典8のスキルへようこそ。</speak>"
            },
            "card": {
                "type": "Simple",
                "title": "技術書典8のスキルへようこそ。",
                "content": "知りたいコンテンツを教えて下さい。"
            },
            "reprompt": {
                "outputSpeech": {
                    "type": "SSML",
                    "ssml": "<speak>知りたいコンテンツを教えて下さい。。</speak>"
                }
            },
            "shouldEndSession": false,
            "type": "_DEFAULT_RESPONSE"
        },
        "sessionAttributes": {
            "STATE": "login"
        },
        "userAgent": "ask-node/2.3.0 Node/v8.10.0"
    }
}
//}

=== モデル精度


この項目はVUIアプリケーション特有の検証項目といえます。VUIアプリケーションでは次の流れで利用者からの音声データがVUIアプリケーションを提供するプラットフォームに渡されます。



//image[architecture][VUIアプリケーションの流れ]{
//}



ここで注目してほしい箇所がASR（音声認識）とNLU（自然言語処理）の部分です。ASRとは、利用者から受け取った音声信号をテキストに変換するための処理です。ASRはプラットフォーム提供者側（AlexaならAmazon社）が管理している部分です。NLUとは、テキストに変換された利用者からの発話内容を、VUIアプリケーションが想定するデータ群に分類するための処理です。この節で述べているモデル精度はNLUの精度となります。NLUの精度が低いと、VUIアプリケーションが想定するインプットのJSONが正しく生成されないため、レスポンスJSONも想定したデータになりません。@<br>{}
　@<br>{}
たとえば、天気をお知らせするVUIアプリケーションがあったとします。「今日の東京都の天気を教えて」といった際、「今日」と「東京」というデータ群をモデルが正確に認識することで、「今日の東京都の天気」を利用者にお知らせできます。しかし、モデルの精度が低い場合、「きょうのとうきょうとのてんきをおしえて」、というASRの結果をNLUに入力した結果、「今日」「きょうと（京都）」というデータ群が取得されたとします。このデータ群をVUIアプリケーションへのインプットデータとすると、応答されるレスポンスJSONはもちろん「今日の京都の天気」となるわけです。これはまさしく想定していない結果です。@<br>{}
そのため、モデルの精度を高くなるように実装をして、それを検証することが重要です。この検証は一般的な開発のユニットテストと同等レベルの検証です。@<br>{}
　@<br>{}
では、どのようにしてモデルの精度の高さを検証するのでしょうか。AlexaではWebの開発者コンソールにさまざまな機能が用意されてます。発話プロファイル（Utterance Profiler）という機能では、ASRされて取得できた音声のテキストデータをインプットとして、NLUによってどのような発話に解釈されるか確認できます。NLU評価ツール（NLU Evaluation）では、利用者から入力する発話音声を複数用意し、それを発話パターンのテストセットとしてモデル学習を実施することでモデルの精度の高さを評価できます。次図では、一部の想定インテントが異なるインテントとして応答されているため評価としては失敗という結果が確認できます。



//image[utteranceProfile][発話プロファイル]{
//}



//image[nluEvaluation][NLU評価ツール]{
//}

=== 実音声による確認


この項目もVUIアプリケーション特有の検証項目といえます。VUIアプリケーションの応答はもちろん音声です。本来この検証項目では、前述した「会話の質の良さ（イントネーション、会話の間、会話の早さなど）」も検証すべきです。しかし、ここで強調したい確認項目は実際の音声による確認時に想定した応答が返ってくることの確認です。レスポンスJSONの検証によって、VUIアプリケーションのロジックレベルの品質は担保されます。モデルの精度の検証によって、NLUの品質は担保されています。この項目でASR→NLU→ロジック→応答までの一連の流れを検証します。この検証は一般的な開発の結合テストと同等レベルの検証です。また、この一連の流れを複数回繰り返して会話を紡ぐことで、シナリオテストと同等レベルの検証ができます。つまり、最終的にはVUIアプリケーションに話しかける形で品質を担保していくことが必須となります。

//note[■困りごと：漢字の読み方の問題]{
レスポンスJSONの検証によって「テキスト」としての応答が正しいことは担保されています。それでも、実音声による確認で誤った応答が発生することもあります。それは漢字の読み方です。「正しく」と書かれたテキストは素直によめば「ただしく」と私は考えています。しかし、文脈によっては「まさしく」と読んでも正しいときがあります。「正しく応答されませんでした」はテキストとして正しいですが、「まさしく応答されませんでした」と応答されたら想定外ですよね。やはり音声応答によるアプリケーションである以上、音が正しいかどうかの確認は必須なのです。
//}


== 自動化によるテスト効率化


ここまで、VUIアプリケーションを構築する上で「なぜ」テストが必要か、「なにを」テストするのかについて述べてきました。本節では、「どのように」テストをするのかについてディーブダイブします。@<br>{}
本節のタイトルに「自動化」という単語があります。ソフトウェア開発において、テストが可能な場合、人手によらない自動テストで実施して品質を担保することは当たり前になっています。これはVUIアプリケーションでも同様です。可能な限りテストを自動化して、テストにかける時間を短くし、効率的によいものを作るべきです。@<br>{}
　@<br>{}
以降では手動テストのつらみと、どこに自動テストを使うべきか、どんな自動テストがあるかについて述べていきます。


=== 手動テストの限界


タイトルで出落ち感がありますが、手動テストにはいくつかの限界があります。たとえば次のような懸念があります。

 * 単位時間あたりに実行できるテスト数が少ない
 * 検証が人手による場合、確認ミスの可能性がある
 * 再現性がない



特に1つ目の「単位時間あたりに実行できるテスト数が少ない」はなかなかつらいです。人の時間は有限です。その貴重な時間をテストで使うのは非常にもったいないです。高速なサイクルで開発と、テストによる品質担保をするためにもテストの速度を上げることは非常に重要です。手動テストではどうしても限界があり、速度を上げようとすると2つ目の確認ミスにつながりやすくなります。こういった作業は機械に任せましょう。そこで自動テストの登場です。


=== 自動テストのメリット、デメリット


次が自動テストのメリット、デメリットです。

 * メリット
 ** 単位時間あたりに実行できるテスト数が多い
 ** 確認ミスが非常に少ない（ゼロではない）
 ** 再現性がある
 * デメリット
 ** 自動テストの環境構築が必要
 ** テストコードのメンテナンスが必要
 ** 適用箇所に限りがある



自動テストのメリットは上述した手動テストのデメリットを解消する形になります。デメリットのうち、注意をしなければいけないのは「適用箇所に限りがある」という点です。すべてに自動テストを適用できるわけではないという点に注意してください。特にVUI、現時点で発売されているスマートスピーカを例に上げると画面付きのものがキワモノとなっています。


=== 自動テスト不可能な項目


現時点では画面付き端末における、画面表示のテストは厳密には自動化ができません。たとえばAlexaの場合、画面表示をするためにはRenderDocumentと呼ばれるJSON形式のレスポンスを画面デバイスに送ることで実現しています。想定される画面表示を実現するJSON形式のレスポンスになっているかどうかで画面表示を確認することはできます。しかし、画面付きデバイスではデバイス側で完結する処理（画面タッチに反応させて別コンテンツを表示するなど）があります。この種類の処理に関しては自動テストができないため、厳密なテストができないとしました。@<br>{}
　@<br>{}
上記以外の項目については自動テストが可能です（ただし、本章で扱わないとした、「会話シナリオの良さ」と「会話の質の良さ」のテストは厳しいです）。次の節で説明するテストフレームワークを用いて、自動テストを積極的に導入してください。


== 自動テストフレームワーク


この節では実際に自動テストを導入するために利用するツール郡の話をします。Alexa特有のものからVUIアプリケーション全般に使えるものまでさまざまです。それぞれのツールを利用するテストの種類も合わせて述べていきます。

=== Mocha＆chai, Jestなど

==== 概要


@<href>{https://mochajs.org/,Mocha}@<fn>{2}とはJavaScriptのテストフレームワークです。@<href>{https://www.chaijs.com/,chai}@<fn>{3}はアサーションライブラリです。テストフレームワークを用いることで、テストの管理とテスト結果の可視化ができます。しかし、純粋なテストフレームワークだけではテストを検証できません。テストのためには、想定したテスト結果と実行したテスト結果を比較するなどして、想定の結果どおりになっているかなどを検証するアサーションライブラリが必要になります。そのため、Mochaを使う場合は別途アサーションライブラリが必要です。Mochaとセットでよく用いられているchaiを使うとよいです。@<br>{}
@<href>{https://jestjs.io/,Jest}@<fn>{4}もJavaScriptのテストフレームワークです。Mochaと違う点としては、フレームワーク内にアサーションライブラリも含まれている点です。「選択肢が多すぎて、テストを書き始める前の段階で止まってしまう」という人はJestを選択してもよいでしょう。@<br>{}
　  
これらのフレームワークはAlexa特有のフレームワークというわけではなく、JavaScriptのテストフレームワークとして語られることが多いです。通常のJavaScript開発でも使えるので知識をつけておいて損はありません。


==== テスト種別


いずれもユニットテストで利用されます。


==== 使い方


非常にシンプルです。対象ライブラリのインストール、テストコードの記述のみとなります。まずはライブラリのインストールについてになります。Mocha、chaiの場合は次のとおりです。


//cmd{
npm install --save-dev mocha chai
//}


jestの場合は次のとおりです。All-in-OneなのでJestのみのインストールになります。


//cmd{
npm install  --save-dev jest
//}


次にテストコードについてです。通常のJavaScriptの場合、次のようなテストコードで実行ができます。


//list[][mocha.test.js][js]{
const expect = require('chai').expect;
const testFunc = (event) => `ようこそ。${event}のためのテストスキルです。`;

describe("LaunchHanlder", () => {
    it("should be OK", () => {
        const result = testFunc("技術書典8");

        expect(result).to.be.a("string");
        expect(result).to.equal('ようこそ。技術書典8のためのテストスキルです。');
    })
}); 
//}


このテストコードを実行すると、次のような結果となり、testFuncという機能のテストが成功していることがわかります。


//cmd[mochaによるテスト実行結果]{
$ npx mocha
    LaunchHanlder
        ✓ should be OK

    1 passing (7ms)
//}


しかし、実際にテストしたいAlexaのコードをMochaやchaiだけで確認することは難しいです。なぜなら、Alexaのコードはシンプルな機能（関数）ではないのです。一般的に、Node.jsにおけるAlexa開発では@<href>{https://developer.amazon.com/en-US/docs/alexa/alexa-skills-kit-sdk-for-nodejs/overview.html,ask-sdk}@<fn>{5}というライブラリを利用してスキルを開発します。ask-sdkを使うと、１つの機能は次のコードで書かれます。


//list[][index.js][js]{
const LaunchRequestHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'LaunchRequest';
    },
    handle(handlerInput) {
        const speakOutput = 'ようこそ。技術書典8のためのテストスキルです。';

        return handlerInput.responseBuilder
            .speak(speakOutput)
            .reprompt(speakOutput)
            .getResponse();
    }
};
//}


このLaunchRequestHandlerという機能はcanHandleという関数とhandleという関数を持ちます。この機能をテストするためにはhandlerInputという引数を渡して関数を実行し、その戻り値を検証する必要があります。このhandlerInputという引数がやっかいです。@<br>{}
通常、検証したい処理と関係のない部分の処理については、処理をモック（模倣）させてテストを通過させます。モックするためには、この引数がどんな関数を持って、どんな処理を返すのかを理解しなければいけません。たとえば、canHandleをテストするためには次の手順が必要です。

 1. handlerInputにrequestEnvelopeというオブジェクトをもたせます。これは、Alexa.getRequestTypeの引数から推測できます。
 1. requestEnvelope関数にrequestというオブジェクトを返却する処理を書きます。なぜなら、Alexa.getRequestTypeの処理内容を見るとrequestEnvelopeという引数で、requestEnvelope.request.typeを返却しているためです。
 1. 2. で作成したオブジェクトにtypeという属性をもたせて、"LaunchRequest"という文字列を設定します。
 1. canHandleをテストするコードを書きます。Alexaスキルのロジックが書かれたコードをインポートして、検証したい処理の結果を取得します。なお、ここで記載のインポート方式は少々ズルをしています。本来のAlexaスキルのコードでは、エクスポートしているのはLambdaHandlerという処理になります。しかし、LaunchRequestHandlerをテストするために、明示的にこの処理だけをエクスポートしています。


//list[][テストコードを通すためのコード][js]{
const launchHandler = require("../lambda/custom/index.js");
describe("LaunchHanlder for Alexa skill", () => {
    it("should be OK", () => {
        const handlerInput = {
            requestEnvelope: {
                request: {
                    type: 'LaunchRequest'
                }
            }
        };

        const result = launchHandler.canHandle(handlerInput);
        expect(result).to.equal(true);
    })
});
//}

//cmd{
$ npx mocha
    LaunchHanlder for Alexa skill
        ✓ should be OK

    1 passing (5ms)
//}


このようにMochaのみでAlexaスキルのテストをすることはたいへん面倒です。これはテストフレームワークをJestにしても同様となります。


==== オススメ度


純粋なJavaScriptのテストフレームワークとしては優秀ですが、Alexa特化というわけではないので、アサーションを自力で書く必要があります。世の中のAlexaで利用するテストフレームワークは、純粋なテストフレームワークをラップしていることが多いです。なぜなら、純粋にMochaやJestを使うとテストが少し面倒だからです。そのため、Alexaという文脈に絞るならば、他のフレームワークを利用したほうがよいです。純粋にこれらのみを使う場合、オススメ度としては低となります。


=== alexa-conversation

==== 概要


@<href>{https://github.com/ExpediaDotCom/alexa-conversation,alexa-conversation}@<fn>{6}は、Alexaスキルのテストフレームワークです。Node.jsで作成したAlexaスキル用のフレームワークとなっています。このライブラリはAlexaのSDKバージョンがv1のころ、よく使われたライブラリです。当初は今ほどAlexaスキルのテストフレームワークが充実しておらず、筆者の周りでは自動化テストをするならalexa-conversationというぐらいになっていました。MochaやJestを純粋に利用することと比較すると、かなりAlexaに特化したテストコードをかけることが魅力でした。


==== テスト種別


ユニットテスト、インテグレーションテストで利用されます。


==== 使い方


対象ライブラリのインストール、テストコードの記述のみとなります。alexa-conversationではテストランナーとしてMochaが使われています。そのため、当該ライブラリに加えてMochaのインストールも必要になります。


//cmd{
npm install --save-dev alexa-conversation mocha
//}


テストコードはMochaなどの純粋なフレームワークと比べて非常にシンプルです。次のように、appというAlexaスキルのオブジェクトを生成し、conversationという命令を開始地点としてコードを書きます。userSays（NLUで解釈された発話結果のインテント）やshouldEqualによるアサーションが可能です。非常にわかりやすくかけますが、看過しがたい問題があります。次のオススメ度の項目をご確認ください。


//list[][alexa-conversation.test.js][js]{
const conversation = require('alexa-conversation');
const app = require('../lambda/custom/index.js');
const opts = {
    name: 'Test Conversation',
    appId: 'amzn1.ask.skill.xxxxx',
    app: app
};

conversation(opts)
    .userSays('LaunchRequest')
    .plainResponse
    .shouldEqual('Welcome back', 'This is the reprompt')
        .end();
//}

==== オススメ度


2020年1月現在、AlexaのSDKがv2になっています@<fn>{7}。この日を堺にGitHubのコミットログが止まっており、メンテナンスされていないことが見受けられます。v2のSDKではそのまま使っても動作しませんでした。現在は豊富なテストライブラリが開発されているため、オススメ度としては低です。他のライブラリを使用することをオススメします。


=== alexa-skill-test-framework

==== 概要


@<href>{https://github.com/BrianMacIntosh/alexa-skill-test-framework,alexa-skill-test-framework}@<fn>{8}は、Alexa特有のテストフレームワークです。ブラックボックステストをするために利用します。テストランナーとしてMochaを利用したライブラリとなっています。


==== テスト種別


ユニットテスト、インテグレーションテストで利用されます。


==== 使い方


対象ライブラリのインストール、テストコードの記述のみとなります。alexa-skill-test-frameworkではテストランナーとしてMochaが使われています。そのため、当該ライブラリに加えてMochaのインストールも必要になります。


//cmd{
npm install --save-dev alexa-skill-test-framework mocha
//}


テストコードは次のようになります。このライブラリの非常にいいところとして、テストコードのサンプルが充実していることです。筆者が今回検証する際にもexamples配下にあるテストコードは非常に参考になりました。また、ライブラリの中身にダイブしてわかったのですが、saysの検証時にssmlタグが不要です。これは、ライブラリ内でsays.substring(7)というtrim処理をしていて、ssmlのタグを排除しているためでした。このあたりが自前のライブラリっぽさがありますね（笑）。


//list[][alexa-skill-tet-framework.test.js][js]{
const alexaTest = require('alexa-skill-test-framework');

alexaTest.initialize(
    require('../lambda/custom/index.js'),
    "amzn1.ask.skill.00000000-0000-0000-0000-000000000000",
    "amzn1.ask.account.VOID");

describe("Hello World Skill", function () {
    describe("HelloWorldIntent", function () {
        alexaTest.test([
            {
                request: alexaTest.getIntentRequest("HelloWorldIntent"),
                says: "技術書典とは技術書の同人誌の祭典です。年に2回開催されますが、今年は豪華に3回も開催されます。ぜひ会場までおこしください！",
                shouldEndSession: true
            }
        ]);
    });

});
//}


なお、本来はLaunchRequestで比較したかったのですが少し問題があったため、インテントリクエストで検証しています。審査では次に応答が期待される場合は疑問符で終わる必要がありますと指摘されることがあります。これをライブラリ側で検知するためか、セッションを閉じていない応答＆疑問符で終わっていないときはアサーションが出てテストがとおりませんでした（笑）。


//cmd[alexa-skill-test-frameworkによるテスト実行結果]{
 Hello World Skill
    LaunchRequest
      1) returns the correct responses


  0 passing (10ms)
  1 failing

  1) Hello World Skill
       LaunchRequest
         returns the correct responses:
     AssertionError: Request #1 (LaunchRequest): Possible Certification Problem: The response keeps the session open but does not contain a question mark.
//}

==== オススメ度


2020年1月現在、頻繁にはコミットをされてはいませんが、SDKのv2は対応しているためツールとしては問題なく使うことができました。応答本体以外の検証もできることは魅力です。しかし、テストコードの箇所が少し煩雑になってしまい、検証したい内容が直感的にわかりにくい印象です。オススメ度としては中となります。



=== ask dialogコマンド

==== 概要


AlexaのCLIツールである「@<href>{https://developer.amazon.com/ja-JP/docs/alexa/smapi/ask-cli-intro.html,Alexa Skills Kitコマンドラインインタフェース（ASK CLI）}@<fn>{9}」のサブコマンドである「dialog」サブコマンドを用いるケースです。ASK CLIは、Alexaスキル、対話モデルやアカウントリンクの詳細といった関連リソースをコマンドラインから管理するために使用できるツールです。Amazonから公式にローンチされているツールであり、テストだけでなく開発や審査、運用などにおいて非常に強力なツールです。今回はその中から、「dialog」サブコマンドを取り上げます。


==== テスト種別


インテグレーションテスト、シナリオテストで利用されます。


==== 使い方


ASK CLIのインストールと初期化が必要です。個人的にはASK CLIはグローバルインストールでもいいですが、ほかのライブラリ同様にローカルインストールにしています。

//cmd{
npm install --save-dev ask-cli
//}

なお、筆者の開発環境はMacなのでnpm installのみで完了していますが、Windowsユーザーの場合注意が必要です。Windowsにインストールする前に、@<tt>{windows-build-tools}パッケージをインストールする必要があります。windows-build-toolsをインストールする前に、パッケージが要求するNode.jsのバージョンがあることを確認してください。windows-build-toolsをインストールするには、最初に管理者として実行オプションを使用してPowerShellを開き、@<tt>{npm install -g --production windows-build-tools}と入力します。@<br>{}


次の初期化処理によって、AmazonアカウントとASK CLIコマンドの関連付けがされます。内部的にはLogin With Amazonによるログイン処理です。ここでコマンドオプションに@<tt>{--profile}をつけています。@<tt>{--profile}をつけない場合、default設定に対してAmazonアカウントが紐付けされます。しかし、ASK CLIは、個人用、仕事用、デモ用など多様なアカウントで使うことが多いです。今回は技術書典８向けの設定を作成しています。@<br>{}


//cmd{
ask init --profile techbookfest8
//}


ここまでの設定でASK CLIが使えます。dialogサブコマンドを使い、次のとおり応答処理が正しく実装されているかインテグレーションテストができます。コマンド上は色がついていて、会話のやりとりが見やすくなっています。



//image[2019-12-31-13-26-20][ask dialogコマンドによるテスト][scale=0.5]{
//}




また、テストのときは再現性が重要でユーザーのそれぞれの入力を毎回実行することが多いです。実行結果をファイルに保存しておくことで、リプレイ処理をしてテストできます。「@<tt>{!record}」で応答を保存、「@<tt>{ask dialog -r ファイル名}」で保存した結果をリプレイできます。



//image[2019-12-31-13-30-07][ask dialogコマンドによるリプレイ処理][scale=0.5]{
//}



==== オススメ度


相手は公式ツールなのでどこまで素直に書くか非常に悩みます（笑）。素直に書くと、本章で取り扱っている「テスト」というコンテキストにおいて、dialogコマンドは非常に有用です。今まではAlexa developer console（次図）のテストタブから実行していたことがローカルのターミナル上からできるためです。しかし、自動テストというコンテキストでとらえると、dialogコマンドを取り入れることは難しいです。なぜなら、コマンドの出力がJSON/YAMLのいずれの形式でもないため、会話した結果を解析したアサーションが難しいためです。リピートオプションなどを使うことで、「単位時間あたりに実行できるテスト数」は手動実行より大きく伸ばすことはできます。しかし、手動確認であることに変わりはないため「確認ミス」という観点は残り続けます。それに引きずられて、「単位時間あたりに実行できるテスト数」が自動テストよりも劣ります。以上の理由から、申し訳ないですが、自動テストにおけるオススメ度は低となります。



//image[2019-12-31-12-59-58][Alexa developer console]{
//}



=== Bespoken Tools

==== 概要


ようやくここまで来ました。これを説明するために、テストとは何か、VUIアプリケーションにおけるテストすべき項目などを書いてきたといっても過言ではありません。@<href>{https://bespoken.io/,Bespoken Tools}@<fn>{10}はBespoken社によるAlexaスキルの開発を効率化するサービスであり、一番有力なテストフレームツールといえます。Alexaの@<href>{https://github.com/alexa/skill-sample-nodejs-fact,サンプルリポジトリ}@<fn>{11}でも、こちらのテストツールを利用されていることから、もっとも有力なテストツールと考えています。


==== テスト種別


ユニットテスト、インテグレーションテスト、シナリオテストのいずれでも利用できます。本章の種別からは少しそれますが、継続的テストやユーザビリティテストも提供しているようです。



//image[2019-12-25-18-39-40][Bespokenで利用できるテスト][scale=0.5]{
//}



==== 使い方


次のようにテスト種別を使い分けています。それぞれについて解説します。


===== Unit testing@<br>{}

LaunchRequestやHelloWorldIntentといった、インテント名を与えて応答を確認するテストです。alexa-conversationやalexa-skill-test-frameworkで実施するテストと同等のテストという認識で問題ありません。@<br>{}



使い方としては、対象のライブラリのインストール、テストコードの記述のみとなります。インストール後はbstコマンドを叩いて、インストールできているか確認するとよいです。ASK CLI同様、グローバルインストールでもいいですが、ほかのライブラリ同様にローカルインストールにしています。テストランナーは内部的にはJestを利用しているように見えます。@<br>{}


//cmd{
$ npm install --save-dev bespoken-tools
**(省略)**
$ ./node_modules/.bin/bst
BST: v2.4.24  Node: v10.17.0
Remember, you can always contact us at https://gitter.im/bespoken/bst.
**(省略)**
//}


次にテストコードを書きます。テストコードを書く際、次のようなディレクトリ構成にしておくとよいです。rootディレクトリにtest/unitディレクトリを作成し、その配下にテストコードのYAMLファイルとテスト設定のJSONをおいています。



//image[2019-12-31-14-08-03][テストコードのディレクトリ構成][scale=0.5]{
//}




実際のテスト設定（JSON）、テストコード（YAML）、実行結果は次のとおりになります。出力がかさむためエラー系はあえて記載しませんが、エラー箇所もわかりやすく表示されます。


//list[][bespokenのテスト設定][json]{
{
  "handler": "../../lambda/custom/index.js",
  "locale": "ja-JP",
  "trace": false,
  "jest": {
    "silent": false
  }
}
//}

//list[][bespokenのテストコード][yaml]{
---
configuration: # 言語設定やモック設定など。日本語スキルなのでja-JPを指定。
    locale: ja-JP

--- # 「---」で始めるのがルール
- test: "LaunchRequestのみ"
- LaunchRequest:
    - response.outputSpeech.ssml: <speak>ようこそ。技術書典8のためのテストスキルです。技術書典、または、ヘルプ、と言ってみてください。どうぞ！</speak>

---
- test: "テスト１. シナリオ: AMAZON.HelpIntent"
- LaunchRequest: # このリクエストへの検証不要な場合、このように記述する
- help: # help, exitレベルの場合、インテント名を書かなくてもいける
  - response.outputSpeech.ssml: <speak>技術書典、と言ってみてください。どうぞ！</speak>
  - response.reprompt.outputSpeech.ssml: <speak>技術書典、と言ってみてください。どうぞ！</speak>
- AMAZON.HelpIntent: # 検証したいリクエストの場合、レベルを下げて検証内容を書く
  - response.outputSpeech.ssml: <speak>技術書典、と言ってみてください。どうぞ！</speak>
- HelloWorldIntent:
  - response.outputSpeech.ssml: <speak>技術書典とは技術書の同人誌の祭典です。年に2回開催されますが、今年は豪華に3回も開催されます。ぜひ会場までおこしください！</speak>
//}

//cmd[bespokenのUnit testing結果]{
$ bst test

BST: v2.4.24  Node: v10.17.0
Use bst launch to mimic someone opening your skill.

 PASS  test/unit/index.test.yml
  ja-JP
    LaunchRequestのみ
      ✓ LaunchRequest (504ms)
    テスト１. シナリオ: AMAZON.HelpIntent
      ✓ LaunchRequest (1ms)
      ✓ help (3ms)
      ✓ AMAZON.HelpIntent
      ✓ HelloWorldIntent

----------|----------|----------|----------|----------|-------------------|
File      |  % Stmts | % Branch |  % Funcs |  % Lines | Uncovered Line #s |
----------|----------|----------|----------|----------|-------------------|
All files |    59.38 |    44.44 |    42.86 |    59.38 |                   |
 index.js |    59.38 |    44.44 |    42.86 |    59.38 |... 98,101,102,104 |
----------|----------|----------|----------|----------|-------------------|
Test Suites: 1 passed, 1 total
Tests:       2 passed, 2 total
Snapshots:   0 total
Time:        1.514s, estimated 2s
Ran all test suites.
//}


テスト結果の表示はJestを利用している人は見慣れたものでしょう。このように、テスト検証だけでなくAlexaスキルのコードのカバレッジも同時に可視化されます。今までのテストコードはJavaScriptで書かれていることが多かったですが、BespokenではYAMLで記述できます。JSONではないのでコメントをかけるためすばらしいです（笑）。ただしJavaScriptと違い、宣言的に記述されるテストコードのため、間に文字列を加工する処理などを挿入はできません。YAMLで記述することによって、JavaScriptのエラーにさいなまれることがない点とどちらを取るかは人によります。@<br>{}
個人的には、テストコードを書く人と実装者が分離されているときはYAMLのほうがよいです。非エンジニアにも読みやすいですし。また、Bespokenを例にすると、Unit testingとE2E testingで同じYAMLの文法によってテストができます。ロジックはJUnit@<fn>{12}、E2EはJMeter@<fn>{13}というように、文法がガラッと変わったりすると学習コストが高いです。YAMLは（拡張YAMLとなると別ですが）ただのYAMLなので学習コストが低いです。ここもYAMLによるテストコードのメリットになるでしょう。


===== End-to-end（E2E）testing@<br>{}

最後にE2Eテスト、文字ベースの入力に対しての応答一致性を確認するテストです。ask dialogコマンドのテストと同等のテストという認識で問題ありません。これにはユニットテストと比べると少し面倒な手順が必要です。E2Eテストをするためには、テキスト化された文字列をNLU（自然言語認識）をして、VUIアプリケーションが想定するデータ群に分解する必要があります。つまり、NLUをさせるための機構が必要になります。ask dialogでは、テキスト入力した結果をクラウドにあるAlexaサービスへ渡してNLUをしています。これはASK CLIがAlexa公式のツールなので実現できているわけです。Bespokenではどうしているかというと、AlexaやGoogle Assistantといったバーチャルデバイスをユーザーが作成して、それがNLUをするように設定して実現しています。一瞬、はてなマークが出ますよね。順を追って理解を深めましょう。まず、具体的には次の図の流れで処理がされます。



//image[bespoken-flow][Bespoken E2E testingのフロー]{
//}




上述にあるように、BespokenにAlexaの機能を使ってもらうために、自身のAmazonアカウントからAlexaへアクセスするための権限を委譲する必要があります。この権限委譲のために必要な処理がバーチャルデバイスの作成になります。バーチャルデバイスを作成することで、Alexaのトークンが発行されます。このトークンをBespokenに渡すことで、BespokenがAlexaサービスを使うための権限を会得し、テストを実行できます。@<br>{}
　@<br>{}
バーチャルデバイスを作成するための手順としては、次のとおりになります。

 * Bespokenのアカウントを作成する。https://apps.bespoken.io/dashboard/login へアクセスして、アカウントを作成します。画像では、Login With Amazonを選択しています。



//image[bespoken-signup][Bespokenアカウントの作成][scale=0.7]{
//}


 * Bespokenがどの情報へアクセスするかの認可画面が出ます。Allowを選択してください。



//image[bespoken-allow][Bespokenアカウントの許可][scale=0.7]{
//}


 * Bespoken Dashboardへログインできます。左タブの上から２番目にあるデバイスマークを選択することでバーチャルデバイスを作成できます。このとき、アンケートみたいな画面に遷移することがあります。サクッと終わるので（翻訳しながら）なんとなく書くとデバイス作成画面に遷移できます。今回はAlexaを選択しましょう。@<br>{}



//image[bespoken-dashboard][Bespoken Dashboard][scale=0.7]{
//}




//image[bespoken-setup][バーチャルデバイスの作成][scale=0.7]{
//}


 * 自身のAmazonアカウントのAlexaサービスを使う必要があるため、再度Amazonアカウントへの認証と認可が求められます。認証をしたら次の画面が出ますので、認可します。



//image[bespoken-auth2][バーチャルデバイス作成時の認可画面][scale=0.7]{
//}


 * 無事、バーチャルデバイスが作成できました。Tokenという列にあるalexa-xxxで始まるIDは後で使うのでコピーしておいてください。



//image[bespoken-virtualdevice][バーチャルデバイス作成完了][scale=0.7]{
//}




長かったですね。ここまでがセットアップです。ではテストをしてみましょう。@<br>{}
まずはテスト設定（JSON）ですが、ユニットテストの設定とかぶらせたくないので、今回はJSONファイルの変更はなしにします。次にテストコードですが、次のようなYAMLファイルを作成します。場所はtest/e2eディレクトリを作成し、その配下などにしましょう。


//list[][E2E Testingのテストコード][yaml]{
---
configuration: # 言語設定やモック設定など。日本語スキルなのでja-JPを指定。
  locale: ja-JP
  voiceId: Mizuki

--- # 「---」で始めるのがルール
- test: シナリオ１、テスト０：スキル起動→終了.
- アレクサ、ベジチャットを開いて: ベジチャットへようこそ切れた野菜を教えてください # 左が発話、右が想定応答
- 終了:
    - prompt:
        - /^$/ # 空応答に相当
//}


今まで呼び出し名「エキスパート本」を用いてましたが、いくつかの理由で「@<href>{https://www.amazon.co.jp/dp/B077LQD523,ベジチャット}@<fn>{14}」というスキルを呼んでいます。この理由はあとで問題点において供述します。テストコードが書けたので実行してみましょう。前述したとおり、テスト設定のJSONファイルは変更していないため、テストコマンドに必要な情報を設定してテストを実行します。typeオプションでE2Eテストを指定、virtualDeviceTokenでBespokenダッシュボードから取得したバーチャルデバイスのToken列にあったIDを設定します。


//cmd[bespokenのe2e testing結果]{
$ bst test --type e2e test/e2e/*.yml --virtualDeviceToken {バーチャルデバイスのTokenID}

BST: v2.4.24  Node: v10.17.0
bst test lets you have a complete set of unit tests using a simple YAML format. Find out more at https://read.bespoken.io.

 PASS  test/e2e/invokingApp.e2e.yml (19.675s)
  ja-JP
    シナリオ１、テスト０：スキル起動→終了.
      ✓ アレクサ、ベジチャットを開いて (9689ms)
      ✓ 終了 (9689ms)

Test Suites: 1 passed, 1 total
Tests:       1 passed, 1 total
Snapshots:   0 total
Time:        19.777s, estimated 24s
Ran all test suites.
//}


無事、E2Eテストが完了しました。今回テストしたテストコードは最小セットに近いです。これ以外にもかなり多様なテストコードやオプションを設定できます。詳細はBespokenの@<href>{https://read.bespoken.io/end-to-end/guide/#bst-test,E2Eテストのページ}@<fn>{15}を確認ください。@<br>{}
　@<br>{}
では最後に現状感じた３つの問題点に触れます。


===== 開発中スキルのテストができない？@<br>{}


ここは筆者も自信がないです。いろいろなオプションを試してみていますが、e2eのtype指定をすると公開されているスキルしか呼び出せない印象です。実際、「エキスパート本を開いて」と発話すると、エキスパートさんの運行情報のスキルが立ち上がります。バーチャルデバイスIDを払い出すときに該当スキルを開発しているAmazonアカウントでOAuthしているにもかかわらず開発スキルが呼べないのは不思議です。type=e2eで開発中スキルのテストができないのは少しつらいのでご存じの方はぜひお知らせください。
 
 

===== 日本語の記号が認識されない@<br>{}


次の図にBespokenダッシュボードから実行したキャプチャを貼り付けています。図の真ん中にある「Expected」の列に書いてある文言が想定文言です。右側あたりにある「Actual」と書かれた列の下にある応答を見ていただきたいです。見事に「、」「。」がない応答を取得しています。カード応答では記号が出ているため、Speech To Text（STT）をした結果に問題があるのでしょう。



//image[bespoken-japanese-error][日本語利用時のBespokenの挙動][scale=0.7]{
//}
  

===== Speech To Text（STT）の精度@<br>{}


最後にSTTの精度についてです。これはBespoken公式のFAQにもかいています。Alexaから返されるオーディオ応答に対して、STTによって想定応答のテキストを生成しているプロセスが完璧ではない模様です@<fn>{16}。E2Eテストで用いたベジチャットスキルを呼び出したYAMLファイルを例に取ります。テストをパスするために、恣意的な想定応答「ベジチャットへようこそ切れた野菜を教えてください」と書いています。本来は、「ベジチャットへようこそ知りたい野菜を教えてください」となります（「。」は先述の問題２のために消しています）。STTせずにAlexaから得た応答をそのままJestに使えばいいのではとなりますが、現状のBespokenのフロー上、STTを通過させているからこその問題となっています。


==== オススメ度


概要でも記述したとおり、公式サンプルが採用していることや機能が非常に豊富な点、継続的テストやE2Eテストも備えていることから将来が期待できるフレームワークです。しかし、YAMLのアウトプットにSSMLタグを含めたり、日本語の機能が不十分なところもあります。これも我々開発者側から改善要望を出していけば良くなっていくでしょう。今後の期待等も込めて、オススメ度は⾼となります。


== おわりに


本章では、VUIアプリケーションにおける、ロジック部分の品質を担保するテストの必要性について書きました。2020年1月現在、日本でローンチされた2017年11月のころの機能と比較するとAlexaはかなり成長を遂げています。それによって、すべての機能を自動テストすることが困難になっています。特に今回は触れなかったAlexa Presentation Language（APL）のテストは非常に難しい状況です。しかし、その一方でテストフレームワークも成長しているため過去は困難であった、テストフェーズのCI/CDの組込みが容易になりました。Bespokenや、NLU評価ツールをうまく開発ワークフローに乗せることで、より効率的なAlexaスキル開発が可能になります。ぜひ、いま実装しているAlexaスキルの開発ワークフローに自動テストを取り入れてみてください。


//footnote[1][Amazon Alexa、Google Assistant、Cortanaなど。]

//footnote[2][https://mochajs.org/]

//footnote[3][https://www.chaijs.com/]

//footnote[4][https://jestjs.io/]

//footnote[5][https://developer.amazon.com/en-US/docs/alexa/alexa-skills-kit-sdk-for-nodejs/overview.html]

//footnote[6][https://github.com/ExpediaDotCom/alexa-conversation]

//footnote[7][https://developer.amazon.com/ja/blogs/alexa/post/51f6f6eb-a6cb-4d2e-82ca-166bdfee61be/ask-sdk-v2-release-japanese]

//footnote[8][https://github.com/BrianMacIntosh/alexa-skill-test-framework]

//footnote[9][https://developer.amazon.com/ja-JP/docs/alexa/smapi/ask-cli-intro.html]

//footnote[10][https://bespoken.io/]

//footnote[11][https://github.com/alexa/skill-sample-nodejs-fact]

//footnote[12][Javaで使われる伝統的なユニットテストランナー]

//footnote[13][伝統的なE2Eテストランナー]

//footnote[14][https://www.amazon.co.jp/dp/B077LQD523]

//footnote[15][https://read.bespoken.io/end-to-end/guide/#bst-test]

//footnote[16][https://read.bespoken.io/end-to-end/faq/#the-response-from-my-skill-is-not-what-i-expected-what-should-i-do]

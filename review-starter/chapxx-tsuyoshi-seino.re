
= 「通知機能」を実装してみよう

//image[tag][]{
//}

//flushright{
著者：せーの（@<href>{https://twitter.com/chao2suke/,@chao2suke}）
//}


== はじめに


あなたは今、Alexaの開発を楽しんでいるだろうか。



ここで「え？楽しんでますけど」という方は、本章はいったん読み飛ばしてしまっても構わない。そういう人はとりあえず上に記されているTwitterに向かって「たいへんおもしろい記事でした！！ 参考になりました！！」とツイートさえしてもらえれば、あとは他の方の章に移ることをお勧めする。本当に読んだかどうかはひとまず横に置いておこう。ハッシュタグで[#AlexaDevs]をつけてもらえるとよりよい。

//blankline

私の所属している会社はAlexaエージェンシーとしてAmazon社とパートナーシップを結んでいる。私はその中でAlexaエバンジェリストとして2019年06月まで活動していた。@<br>{}
要するに仕事としてAlexaの最新情報をいち早くキャッチアップして、それをスキル開発者や学生さん、企業のエンジニアに噛み砕いて伝える、という作業をしていた。ちなみに私の表記が「せーの」なのは、こういったプレゼンやカンファレンスの場にて漢字で「清野」と書くと9割近い確率で「きよのさん」と呼ばれるため、それを毎回「あ、いや、あのー・・・」と苦笑いで訂正する手間を省くための業務効率化の結果だ。@<br>{}
これは現在のように女優の清野菜名さんがドラマの主演になったり、漫画家の清野とおるさんが壇蜜さんと結婚したりして「清野 = せーの」というイメージが一般の人にできる前の時代の話だ。@<br>{}
そして時は2020年、長らく「きよの」の影に隠れていた「せーの」たちが、密かに研いでいた牙をとうとう露わにして清野の利権を奪うべく全面戦争をしかける時がやってくることになるわけだが、本章にはまったく関係がないことなので割愛する。

//blankline

そんなどっぷりAlexa漬けの日々を過ごしていると、一般的なスキル開発などはすっかり「作業」となってしまい、どうしても新鮮さを失ってしまう。



そんな時にできた機能がこの「通知機能」だった。



それまでAlexaは「Alexaの側からは話しかけない」という大前提が仕様としてあった。家にあるスマートスピーカーが突然話し出すことを受け入れるほど時代は進んでいないからだ。そもそもEchoが話しだした時、周りに誰もいなかったら大切なメッセージを聞き逃すかもしれない。そういった理由でAlexa事業に近ければ近い人間ほど、そういったプッシュ的な機能はAlexaにはつかない、と思っていた。



このインパクトは私にとってはとても大きく、Alexaスキル開発に初めて触れた時と同じような興奮を味わった。この機能を経てAlexaは次のステップを踏み出した、と言ってもいい。それくらいの大革命だったのだ。

//blankline

またこの通知機能に使われている基本的なしくみはこの後に出てくるさまざまなAlexaの機能の元になっている。これは後述する。



「通知機能」は、実装自体はとてもシンプルなものだ。基本的なしくみとAPIを覚えてしまえばすぐに使えるようになる。@<br>{}
本章を通してぜひ、一歩進んだAlexaスキルを開発する楽しさを味わってほしい。


===== 本章の対象読者@<br>{}

本章の対象読者は、次のような方です。

 * 一通りAlexaスキルの基礎的な開発方法を覚えた方
 * セールでEcho Dotを激安で買ってはみたものの、いざ何を話しかけたらいいかわからない方
 * 現在自分が生活の糧として身についている技術以外にもうひとつ身につけたいと考えている方

===== 本章の前提となる知識@<br>{}

本章を読み進めるにあたり、次の基礎知識が必要です。

 * JavaScript
 * Alexaスキル開発の基礎的な知識（インテント、スロットなど）
 * @<href>{https://developer.amazon.com/ja-JP/alexa,Alexa開発者コンソール}の基本的な使い方

===== 本章の必要環境@<br>{}

本章を読み進めるにあたり、次の環境が必要です。

 * PC / mac
 * ブラウザ Google Chrome / Firefoxの最新版
 * Echoデバイス


//clearpage



== 通知機能でできること/できないこと

=== 通知機能とは


ここではまず最初に、そもそも「通知機能」というのは何をどこまで通知してくれるのか、あらためておさらいしよう。


==== ショッピング通知


家にEchoデバイスがあるご家庭で、Amazonで買い物をした時にEchoが黄色にじんわり光ることはないだろうか。



//image[proactiveapi0-1][通知がきた時のEcho][scale=0.5]{
//}




この黄色に光るリングやポン、という効果音が「通知機能」だ。@<br>{}
いや、リングは緑に光っているぞ、というあなた。それは別のAlexaデバイスと通話がつながっている。通知機能ではないのでさよならを言って通話を切ろう。白く光っているあなた、それは音量がMAXになっている。そのままSpotifyを鳴らすと飼っている犬がビクッとするので音量を下げよう。通知機能は「黄色」に光る。



この黄色に光ったAlexaに向かってユーザーから「Alexa, 通知は何？」と話しかけるとAlexaより「Amazonからお届け物が今日届きます」といったメッセージが流れる。荷物の届く日がわかって大変便利な機能だ。

//blankline

ここで重要なことは「通知機能においてはいきなりAlexaが話し出すわけではない」ということだ。つまりUX@<fn>{annotation1}を考える際においてはAlexaから話し出すようなフローは省くべきだ。通知はユーザーがその内容を聞くまでそのままであり、情報がリアルタイムでユーザーに届くとは限らない。また通知が重なる場合もあり、その際に通知内容をどのように管理するのか、に配慮が必要となる。このあたりのフローについては後述することとする。

//footnote[annotation1][User Experience。ユーザーがサービスなどを通じて得る体験のこと。ユーザー体験。]

//blankline

ちなみにAlexaアプリにはすでにAmazonショッピングでの通知について細かい制御機能がついているので見てみよう。



Alexaアプリから[設定]から、[通知]という項目をタップする。



//image[proactiveapi0-2][通知画面][scale=0.5]{
//}




[Amazonでお買い物]をタップする。



//image[proactiveapi0-3][お買い物画面][scale=0.5]{
//}




ここでは荷物が配送されるどのタイミングで通知を受け取るかを選択できる。家に家族が常にいるような過程であれば「配達済み」だろうし、一人暮らしであれば「配送中」が便利だ。@<br>{}
また、プレゼントなどで商品名を読み上げてほしくない場合は、[商品名の読み上げと表示]のセクションを設定することで、商品名読み上げの制御ができる。通知内容がここまで制御できると使い勝手は格段に上がる。


==== リマインダ


現在のAlexaで通知機能を体験できる一番手軽な方法は「リマインダ」だろう。リマインダは「Alexa、30分後に【時間だよ】とリマインダして」のようにリマインダする日時、リマインダの内容をAlexaに伝えると、その時刻に通知が来る、というものだ。



//image[proactiveapi0-4][リマインダ画面][scale=0.75]{
//}




通知が来た際にはAlexaアプリにも同様の通知が来る。ちなみにリマインダは個別のAlexaデバイスでの機能となるので、同じAlexaアカウントでも他のAlexaデバイスに通知が来ることはない。



これもAlexaの代表的な通知機能だ。忘れたくない用事が出来た際に手軽に利用できて大変便利である。



Alexaのビルトイン機能の他にも通知機能を使っているスキルは沢山ある。
代表的なものでいえばタクシー配車スキル「JapanTaxi」は配車が決まった時と実際にタクシーがくる数分前に通知が来る。お天気スキルの「Yahoo!天気・災害」はその日雨や雪の予報の場合は朝７時に通知が来る仕様となっている。必要な情報を必要なタイミングでユーザーに知らせたい時、通知機能を使うとユーザー体験が格段に上がる。



ではこの通知機能を自分で作成したカスタムスキルに取り入れるにはどうしたらいいか。実装には「Proactive Events API」というAPIを使う。
その仕組みをこれから見ていこう。



//clearpage



== Proactive APIのしくみ


Proactive Events APIは次のような仕組みで動いている。



//image[proactiveapi1-1][ProactiveEventsAPIのしくみ][scale=1]{
//}




Proactive Events APIで通知できるイベントはいくつかの固定のパターンに分かれている。これをスキーマと呼び、スキーマにない形のイベントは現状残念ながら呼び出すことはできない。イベントは「マニフェストファイル」と呼ばれる設定ファイルに記述され、Alexaに設定される。@<br>{}
マニフェストファイルにて設定されたイベントはPub/Subと呼ばれる仕組みにてメッセージングされる。



Pub/Subとは主にIoTの世界で使われる仕組みでPublisher（配信者）とSubscriber（購読者）の2つからなるメッセージングを指す。主にMQTTという通信規格にて使用する。  



//image[proactiveapi1-2][mqtt][scale=0.75]{
//}




最初にメッセージが欲しいトピックに対してSubscriberがSubscribe（購読）を登録する。Publisherがそのトピックに対してPublish（配信）をすると、自動的にSubscriberにそのメッセージが届く、という仕組みだ。@<br>{}
現実の世界では新聞購読の仕組みを想像するとよいだろう。読者はまず特定の新聞社の新聞をまず購読するように申し込む。そうすると購読を申し込みしている家庭に販売店から一斉に同じ新聞が届く。購読を申し込んでいる家庭は申込みさえしていれば、必ず毎朝同じ時間に情報を受け取ることができる。



Proactive Events APIでは外部からのリクエストがPublisher、スキルがSubscriberにあたる。まずスキル側で欲しい通知イベントをSubscribeしておく。そうすると、そのイベントが外部からAlexaにPublishされたタイミングで自動的にSubscribeされたスキルに通知される、という仕組みだ。



つまり、Alexaの通知機能はスキル側（Subscriber）がイベントをSuscribeしておけば、後はどこからPublishしてもスキルに通知が届く。PCやWebアプリはもちろん、モバイルアプリ、IoT機器からのアクションなど、どのようなデバイスからでもAlexaに通知することが可能だ。この柔軟性がProactive Events APIの最大の特徴だ。


=== マニフェストファイルの書き換え


マニフェストファイルは「SMAPI（Skill Manifest API）」というAPIを使って書き換える。SMAPIを使うには対象となるスキルの「スキルID」が必要だ。また簡単にSMAPIを使うために「ASK CLI」というコマンドツールがAmazonから用意されているのでそれを使う。



まずはスキルIDの取得をしてみよう。Alexaの開発者コンソールを開き、適当なスキルを作成する。



開発者コンソールを開いたら「スキルの作成」ボタンをクリック。



//image[proactiveapi5][スキルID取得1][border=on]{
//}




スキル名に「プロアクティブイベントサンプル」と入力し、モデルをカスタム、ホストをAlexaに設定して「スキルを作成」ボタンをクリックする。



//image[proactiveapi6][スキルID取得2][border=on]{
//}




テンプレートを適当に選択する。



//image[proactiveapi7][スキルID取得3][border=on]{
//}




しばらく待つとスキルとバックエンドの枠組みが同時に生成される。



//image[proactiveapi8][スキルID取得4][border=on]{
//}




スキルが作成できたらいったん選択画面に戻ろう。左上のロゴをクリックする。



//image[proactiveapi9][スキルID取得5][border=on]{
//}




スキルの選択画面から「スキルIDの表示」をクリックするとスキルIDがポップアップされる。



//image[proactiveapi10][スキルID取得6][border=on]{
//}




表示されたスキルIDをコピーしておこう。



次にASK CLIのインストールだ。Windowsならコマンドプロンプト、macならターミナルを開いて次のように入力する。


//emlist{
npm install -g ask-cli
//}


この際macを使用している方はXCodeのアップデートを完了していない場合エラーになることがある。


//emlist{
gyp: No Xcode or CLT version detected!
//}


のようなエラーが出た場合はXCodeを開き、未インストールとなっているコンポーネントをインストール後、もう一度インストールするとエラーなく完了する。



インストールが終わったら現在のスキルのマニフェストファイルを見てみよう。
次のコマンドを叩いてみよう


//emlist{
ask api get-skill -s amzn1.ask.skill.xxxxx-xxxx-xxxx > skill.json
//}


「amzn1.ask.skill.xxxxx-xxxx-xxxx」の部分はさきほどコピーしたスキルIDに置き換える。成功するとプロンプトでカレントディレクトリにあたるパスにskill.jsonというファイルができているはずだ。中身を開くとこのようなJSONが表示される。


//emlist{
{
  "manifest": {
    "apis": {
      "custom": {
        "endpoint": {
          "uri": "arn:aws:lambda:us-east-1:XXXXXXXXXXXXXXXXXXX"
        }
      }
    },
    "manifestVersion": "1.0",
    "publishingInformation": {
      "locales": {
        "ja-JP": {
          "name": "プロアクティブイベントサンプル",
          "summary": "Sample Short Description",
          "description": "Sample Full Description",
          "examplePhrases": [
            "Alexa open hello world",
            "hello",
            "help"
          ],
          "keywords": []
        }
      },
      "isAvailableWorldwide": true,
      "testingInstructions": "Sample Testing Instructions.",
      "category": "KNOWLEDGE_AND_TRIVIA",
      "distributionCountries": []
    }
  }
}

//}


これがマニフェストファイルだ。ここに後述する「パーミッション」と「イベントスキーマ」を追加して更新するとProactive Events APIが使えるようになる。


=== パーミッション


パーミッションとはAlexaのいろいろな機能を使うことをAlexaが許可する、と明示的に示すために使うセクションだ。@<br>{}
今回は通知機能を使うパーミッションを追加する。次のようなセクションをマニフェストファイルのトップレベルに追記する。


//emlist{
"permissions": [
      {
        "name": "alexa::devices:all:notifications:write"
      }
    ],
//}

//emlist{
{
  "manifest": {
    "apis": {
      "custom": {
        "endpoint": {
          "uri": "arn:aws:lambda:us-east-1:XXXXXXXXXXXXXXXXXXX"
        }
      }
    },
    "manifestVersion": "1.0",
    "permissions": [
      {
        "name": "alexa::devices:all:notifications:write"
      }
    ],
    "publishingInformation": {
      "locales": {
        "ja-JP": {
          "name": "プロアクティブイベントサンプル",
          "summary": "Sample Short Description",
          "description": "Sample Full Description",
          "examplePhrases": [
            "Alexa open hello world",
            "hello",
            "help"
          ],
          "keywords": []
        }
      },
      "isAvailableWorldwide": true,
      "testingInstructions": "Sample Testing Instructions.",
      "category": "KNOWLEDGE_AND_TRIVIA",
      "distributionCountries": []
    }
  }
}
//}

=== パーミッションのフィールド


パーミッションの中にはnameというを含む必要がある。ここにはどのような権限をAlexaに追加するか、を指定している。通知機能に使うフィールドは


//emlist{
alexa::devices:all:notifications:write
//}


の1種類なので、通知機能を使いたい場合はブルース・リーのごとく何も考えず、この値を書き込むだけでいい。


=== イベントスキーマ


イベントスキーマとは課金やアカウントリンクなど、Alexaにおけるさまざまなイベントを管理するJSONセクションとなり、Proactive Eventsを使うときもProactive Eventsのイベントスキーマをマニフェストファイルに追加し、更新する必要がある。Proactive Eventsのスキーマは具体的に



ユーザー 「Alexa、通知は何？」



Alexa 「新しい通知が1件あります。◯◯（スキル名)から。」



Alexa「<スキーマ メッセージ>」



この部分のメッセージを指し、それぞれ会話の型（テンプレート）が決まっている。ではひとつずつ見ていこう。


==== AMAZON.WeatherAlert.Activated


主に天気予報などに使う、天気の注意報や警報を通知するテンプレートだ。
通知内容はこのようになる。



「この地域に<weatherAlert.alertType>が発令されています。この情報は<weatherAlert.source>がお送りしています」



weatherAlert.sourceを指定しない場合、「この情報は〜」以降が省略される。


==== AMAZON.SportsEvent.Updated


サッカーイベントの最新スコアを通知するテンプレートだ。
通知内容はこのようになる。



「<sportsEvent.eventLeague.name>の試合速報です。<update.teamName>が得点しました。<sportsEvent.awayTeamStatistic.team.name><sportsEvent.awayTeamStatistic.score>点、<sportsEvent.homeTeamStatistic.team.name><sportsEvent.homeTeamStatistic.score>点です」


==== AMAZON.MessageAlert.Activated


メッセージのリマインダーを通知するテンプレートだ。
通知内容はこのようになる。



「<messageGroup.creator.name>から、<messageGroup.count>件の<state.freshness>、<state.status>、<urgency>のメッセージが届いています」


==== AMAZON.OrderStatus.Updated


注文の最新情報を通知する時などにつかうテンプレートだ。
通知内容はOrderStatusというパラメータの違いによってこのようになる。

 * PREORDER_RECEIVED： <order.seller.name>での予約注文を受け付けました。
 * ORDER_RECEIVED： <order.seller.name>でのご注文を受け付けました。
 * ORDER_PREPARING： <order.seller.name>でのご注文は出荷準備中です。
 * ORDER_SHIPPED： <order.seller.name>でのご注文は出荷済みです。<state.deliveryDetails.expectedArrival>までのお届け予定です。
 * ORDER@<b>{OUT}FOR_DELIVERY： <order.seller.name>でのご注文は配達中です。<state.deliveryDetails.expectedArrival>までのお届け予定です。
 * ORDER_DELIVERED： <order.seller.name>でのご注文は<state.deliveredOn>に配達を完了しました。
 * ORDER_DELIVERED： <order.seller.name>でのご注文は配達を完了しています。


==== AMAZON.Occasion.Updated


予約の確認に使うテンプレートだ。
通知内容はこのようになる。



「<occasion.bookingTime>の<occasion.provider.name>での<occasion.subject>の<occasion.occasionType>が<state.confirmationStatus>されました。『<skill name>で確認して』と言ってください」


==== AMAZON.TrashCollectionAlert.Activated


ゴミ収集の際に通知するテンプレートだ。
通知内容はこのようになる。



「次の<alert.collectionDayOfWeek>は、<alert.garbageTypes*>の収集日です」


==== AMAZON.MediaContent.Available


メディア・コンテンツ、つまり音楽や絵画、動画コンテンツなどが利用可能になる旨を通知するテンプレートだ。
通知内容はこのようになる。



「<content.name>は、<availability.startTime>に<availability.provider.name>で<availability.method>されます」


==== AMAZON.SocialGameInvite.Available


ゲームの招待通知をするためのテンプレートだ。
通知内容はこのようになる。



「<invite.inviter.relationshipToInvitee>の<invite.inviter.name>から、<game.name>の<game.offer>に<invite.inviteType>されました」



それぞれのテンプレートのパラメータなどの詳細はAmazonのWebサイト（ https://developer.amazon.com/ja-JP/docs/alexa/smapi/schemas-for-proactive-events.html ）を参考にするとよいだろう。



このイベントスキーマをeventsという名前でパーミッションと同じくマニフェストのトップレベルに追記する。


//emlist{
"events": {
      "publications": [
        {
          "eventName": "AMAZON.TrashCollectionAlert.Activated"
        }
      ],
      "endpoint": {
        "uri": "arn:aws:lambda:us-east-1:xxxxxxxxx:function:sample"
      },
      "subscriptions": [
        {
          "eventName": "SKILL_PROACTIVE_SUBSCRIPTION_CHANGED"
        }
      ]
    },
//}

//emlist{
{
  "manifest": {
    "apis": {
      "custom": {
        "endpoint": {
          "uri": "arn:aws:lambda:us-east-1:2xxxxxxxx:function:sample"
        }
      }
    },
    "manifestVersion": "1.0",
    "permissions": [
      {
        "name": "alexa::devices:all:notifications:write"
      }
    ],
    "events": {
      "publications": [
        {
          "eventName": "AMAZON.TrashCollectionAlert.Activated"
        }
      ],
      "endpoint": {
        "uri": "arn:aws:lambda:us-east-1:xxxxxxxxx:function:sample"
      },
      "subscriptions": [
        {
          "eventName": "SKILL_PROACTIVE_SUBSCRIPTION_CHANGED"
        }
      ]
    },
    "publishingInformation": {
      "locales": {
        "ja-JP": {
          "name": "プロアクティブイベントサンプル",
          "summary": "Sample Short Description",
          "description": "Sample Full Description",
          "examplePhrases": [
            "Alexa open hello world",
            "hello",
            "help"
          ],
          "keywords": []
        }
      },
      "isAvailableWorldwide": true,
      "testingInstructions": "Sample Testing Instructions.",
      "category": "KNOWLEDGE_AND_TRIVIA",
      "distributionCountries": []
    }
  }
}
//}

=== イベントスキーマのフィールド


イベントスキーマの中には大きく分けて「publications」「endpoint」「subscriptions」という3つを書き込む必要がある。


==== publications


Proactive eventsで使うテンプレート名を指定する。


==== endpoint


イベントを送るエンドポイントをuri、またはARN(Amazon Resource Name。AWSサービスを使用するさいのサービスエンドポイントのこと）形式で指定する。開発者コンソールの「エンドポイント」に「デフォルトの地域」として指定するARNをそのまま指定すればよい。



//image[proactiveapi11][endpointの取得][border=on]{
//}



==== subscriptions


ここにはスキルに対してユーザーがどのようなアクションをした時にこの通知を受け取るか、という情報を指定する。通知機能の場合は


//emlist{
SKILL_PROACTIVE_SUBSCRIPTION_CHANGED
//}


という1種類なので、長嶋茂雄のようにsubscriptionsを書く時が来たら、そのままこのように書けばよい。



ちなみにこれは前述したPub/Subの仕組みでいう「Publisher」「Subscriber」の設定と同じ意味をもつ。@<br>{}
つまり先にsubscriptionsに書かれているSKILL@<b>{PROACTIVE}SUBSCRIPTION_CHANGED、つまりユーザーがAlexaのスキル画面で通知機能を許可する、というアクションをした際にこのスキルはSubscriberとして登録され、publicationsに指定されているテンプレート名のイベントがPublish、実行された瞬間にこのスキルにメッセージが届くようになるのだ。



//image[proactiveapi14][subscriptions][scale=0.5]{
//}




それではこれからeventsの具体的な中身の実装方法を解説し、実際にスキルを動くようにしてみよう。


//clearpage



== 【実装してみよう】


ここまででProActive APIの仕組みが大分理解できたかと思う。
ここでテンプレートの中からゴミ収集に関するスキルを実装してみよう。



ゴミ収集で使うテンプレートは「AMAZON.TrashCollectionAlert.Activated」となる。
さっそく組みながら解説していこう。


=== マニフェストファイルの値を確認する


前項にて、さきほどのマニフェストファイルにパーミッション、およびイベントスキーマを追記した。@<br>{}
その値を今一度確認してみよう。特にイベントスキーマのendpointの部分はサンプルでは伏せられているので、各々の開発者コンソールからLambdaのエンドポイントをコピーして埋めるようにしよう。



//image[proactiveapi11][endpointの取得][border=on]{
//}



=== マニフェストファイルの更新


値がすべて整っていたら、マニフェストファイルを更新する。プロンプトで次のように打とう。


//emlist{
ask api update-skill -s amzn1.ask.skill.xxxxx-xxxx-xxxx -f skill.json
//}


更新が終わったら確認のためにもう一度マニフェストファイルを確認してみよう。


//emlist{
ask api get-skill -s amzn1.ask.skill.xxxxx-xxxx-xxxx
//}


プロンプト上に表示されたJSONにpermission, eventsセクションが追加されていれば成功だ。


=== 送信側実装


つぎにAMAZON.TrashCollectionAlert.Activatedイベントを送信するスクリプトを書いてみよう。@<br>{}
このスクリプトではAPI送信のために「アクセストークン」を取得する必要がある。


==== アクセストークンを取得する


アクセストークンはサービスの「clientId」と「clientSecret」を元に専用のAPIで呼び出せば取得できる。clientIdとclientSecretは開発者コンソールに表示されている。



まずは開発者コンソールから「アクセス権限」というページにアクセスする。



//image[proactiveapi12][アクセストークンの取得1][border=on]{
//}




clientId、clientSecretはアクセス権限の一番下にある。スクロールが二重になっているので、一度この情報の前でスクロールが止まるが、そのまま下までスクロールすると出てくる。



//image[proactiveapi13][アクセストークンの取得2][border=on]{
//}




このcliendId、clientSecretをコピーする。clientSecretは伏せ字になっているので、横の「表示」ボタンで表示する。



clientId、clientSecretを取得したら、AmazonのAPIサーバを使ってアクセストークンを取得するスクリプトを書く。


//emlist{
function getToken(clientId, clientSecret) {
    const uri = 'https://api.amazon.com/auth/o2/token'

    let body = 'grant_type=client_credentials';
    body += '&client_id=' + clientId;
    body += '&client_secret=' + clientSecret;
    body += '&scope=alexa::proactive_events';

    const options = {
        method: 'POST',
        uri: uri,
        timeout: 30 * 1000,
        body: body,
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded'
        }
    };
    const data = await rp(options);
    return JSON.parse(data).access_token;
}
//}


Amazon.comのAPIサーバに対してProactive events APIをたたく権限を含んだアクセストークンを取得するようにリクエストを投げている。


==== ゴミ収集の送信内容を作成する


次にゴミ収集の送信内容をJSON形式で作成するスクリプトを書く。


//emlist{
function trashCollectionAlertEvent() {

    let timestamp = new Date();
    let expiryTime = new Date();
    expiryTime.setMinutes(expiryTime.getMinutes() + 60);

    return {
        'timestamp': timestamp.toISOString(),
        'referenceId': 'id-'+ new Date().getTime(),
        'expiryTime': expiryTime.toISOString(),
        'event': {
            'name': 'AMAZON.TrashCollectionAlert.Activated',
            'payload': {
                'alert': {
                    'garbageTypes': [
                        'PET_BOTTLES',
                        'RECYCLABLE_PLASTICS',
                        'WASTE_PAPER',
                        'COMPOSTABLE',
                    ],
                    'collectionDayOfWeek': 'TUESDAY'
                }
            }
        },
        'localizedAttributes': [
            {
                'locale': 'ja-JP',
                'sellerName': 'TrashCollection'
            }
        ],
        'relevantAudience': {
            'type': 'Multicast',
            'payload': {}
        }
    }
}
//}


ゴミ収集の送信内容はそれぞれこのようなパラメータで構成されている。

 * alert: ゴミ収集の内容を表す。gabageTypesとcollectionDayOfWeekの2つで構成される。
 ** gabageTypes: ゴミの種類(BOTTLES、BULKY、BURNABLE、CANS、CLOTHING、COMPOSTABLE、CRUSHABLE、GARDEN@<b>{WASTE、GLASS、HAZARDOUS、HOME}APPLIANCES、KITCHEN@<b>{WASTE、LANDFILL、PET}BOTTLES、RECYCLABLE@<b>{PLASTICS、WASTE}PAPER）
 ** collectionDayOfWeek: ゴミを出す日（MONDAY、TUESDAY、WEDNESDAY、THURSDAY、FRIDAY、SATURDAY、SUNDAY）



これらをpayloadというセクションでくくり、名前や日時などの付帯情報をつければ完成だ。


==== APIを送信する


最後に取得したトークンを使い、Alexaに向かってProactive events APIをたたく部分のスクリプトを書く。


//emlist{
function sendEvent(token) {

    const body = JSON.stringify(trashCollectionAlertEvent());
    const uri = 'https://api.fe.amazonalexa.com/v1/proactiveEvents/stages/development'
    //const uri = 'https://api.fe.amazonalexa.com/v1/proactiveEvents/' // 公開スキルでは、こちら

    const options = {
        method: 'POST',
        uri: uri,
        timeout: 30 * 1000,
        body: body,
        headers: {
            'Content-Type': 'application/json',
            'Content-Length': body.length,
            'Authorization' : 'Bearer ' + token
        }
    };
    await rp(options);
}
//}


注意点としては「公開前」と「公開後」ではAPIを送信するエンドポイントが異なる、という点だ。
さらに送信エンドポイント（api.fe.amazonalexa.com）の「fe」というサブドメインはfar eastの略、つまりこのAPIは日本リージョンにあるスキルに対して送る場合のuriだ。これがヨーロッパリージョンの場合はapi.eu.amazonalexa.com、となる。  



あとは通常のAPIをたたくのとほぼ変わらない作法だ。アクセストークンはBearerで設定する。body部分のJSONに余計なダブルクオーテーションなどがついているとJSON.stringify()メソッドでエスケープされてしまうので、bodyの中身はシングルクオーテーションで記述するように注意しよう。



これらのスクリプトを組み合わせるとこのようになる。


//emlist{
const rp = require('request-promise');

const clientId = process.env.CLIENT_ID;
const clientSecret = process.env.CLIENT_SECRET;

notify(); 

async function notify() {
    const token = await getToken(clientId, clientSecret);
    await sendEvent(token);
}

async function getToken(clientId, clientSecret) {
    const uri = 'https://api.amazon.com/auth/o2/token'

    let body = 'grant_type=client_credentials';
    body += '&client_id=' + clientId;
    body += '&client_secret=' + clientSecret;
    body += '&scope=alexa::proactive_events';

    const options = {
        method: 'POST',
        uri: uri,
        timeout: 30 * 1000,
        body: body,
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded'
        }
    };
    const data = await rp(options);
    return JSON.parse(data).access_token;
}

async function sendEvent(token) {

    const body = JSON.stringify(trashCollectionAlertEvent());
    const uri = 'https://api.fe.amazonalexa.com/v1/proactiveEvents/stages/development'
    //const uri = 'https://api.fe.amazonalexa.com/v1/proactiveEvents/' // 公開スキルでは、こちら

    const options = {
        method: 'POST',
        uri: uri,
        timeout: 30 * 1000,
        body: body,
        headers: {
            'Content-Type': 'application/json',
            'Content-Length': body.length,
            'Authorization' : 'Bearer ' + token
        }
    };
    await rp(options);
}

function trashCollectionAlertEvent() {

    let timestamp = new Date();
    let expiryTime = new Date();
    expiryTime.setMinutes(expiryTime.getMinutes() + 60);

    return {
        'timestamp': timestamp.toISOString(),
        'referenceId': 'id-'+ new Date().getTime(),
        'expiryTime': expiryTime.toISOString(),
        'event': {
            'name': 'AMAZON.TrashCollectionAlert.Activated',
            'payload': {
                'alert': {
                    'garbageTypes': [
                        'PET_BOTTLES',
                        'RECYCLABLE_PLASTICS',
                        'WASTE_PAPER',
                        'COMPOSTABLE',
                    ],
                    'collectionDayOfWeek': 'TUESDAY'
                }
            }
        },
        'localizedAttributes': [
            {
                'locale': 'ja-JP',
                'sellerName': 'TrashCollection'
            }
        ],
        'relevantAudience': {
            'type': 'Multicast',
            'payload': {}
        }
    }
}
//}


clientId、clientSecretの部分を環境変数に書き換えているが、サンプルなので動かす時はそのまま直接記述してしまって構わない。



スクリプトが完成したら保存し、適当に作ったフォルダの中に入れて実行権限をつけよう。


//emlist{
mkdir proactiveSample
mv index.js proactiveSample/
cd proactiveSample
chmod a+x index.js
//}


request-promiseというモジュールを使うのでnpmを使ってインストールする。


//emlist{
npm install request request-promise
//}


これで準備はOKだ。


=== テスト


ではテストをしてみよう。まずはテストするAlexaの設定画面よりスキルの通知設定を許可する。Alexaアプリより「スキルとゲーム」を開き「有効なスキル」をタップする。



//image[proactiveapi15][テスト1][scale=0.4,border=on]{
//}




「開発」という枠をタップする。



//image[proactiveapi17][テスト2][scale=0.4,border=on]{
//}




すると開発中のスキルの中に今回作ったスキルが表示されているのでそれをタップする。



//image[proactiveapi16][テスト3][scale=0.4,border=on]{
//}




「有効にして使用する」をタップする。



//image[proactiveapi18][テスト4][scale=0.4,border=on]{
//}




「スキルのアクセス権」として通知機能のチェックが出てくるので、チェックを入れて「アクセス権を保存」をタップする。これで通知機能の許可がAlexaサーバーに飛び、先にあげたマニフェストファイルのsubscriptionsをAlexaが参照してProactive events APIに対するSubscriberにこのスキルが登録される。



//image[proactiveapi19][テスト5][scale=0.4,border=on]{
//}




最後にさきほど作ったスクリプトを実行してみよう。プロンプトからindex.jsをnodeで実行する。


//emlist{
node index.js
//}


開発中のAlexaアカウントで入っているEchoデバイスから通知音が鳴り、音が出たら成功だ。@<br>{}
「Alexa、通知は何？」と聞いてみよう。ゴミの日をおしえてくれるはずだ。


//clearpage



== メッセージパターンの応用


さて、実はこの通知機能に使われているアーキテクチャはAlexaにおいて重要なパターンとなる。@<br>{}
というのも、あるスキル内のアーキテクチャと、その「外」にある仕組みをつなぐ際は、ほとんどこのような形を取るからだ。



あらためて通知機能のアーキテクチャをおさらいしてみよう。通知機能はこのような形で構成される。



//image[proactiveapi1-1][ProactiveEventsAPIのしくみ]{
//}




ここで注目すべきは「通知を送る側」と「通知を受ける側」はPub/Subの仕組みが使われていて、関連性がない、という部分だ。つまり、通知を受ける側のスキルはアクティブな状態ではなく、セッションも切れている。通知を送る側からのイベントはスキルではなくAlexaそのものが検知して対応している。なので通知を受けてEchoが話している内容はAlexaから生成されたもので、スキルの実装ではない。今回のサンプルでもスキル側のLambdaには一切コードを書いていないが通知機能は動くのだ。



Alexaのほかのメッセージングにもこれらのパターンが応用されている。  



通知機能では通知イベントを受け取ったAlexaは独自に応答を返し、スキルにメッセージを送ることはない。だがこれを応用し、スキル内から別のソリューションにイベントを送り、その結果をスキル内で受け取って続きを行う、という仕組みをいくつか紹介しよう。


==== Skill Connections


Skill Connectionsはあるスキルの中から別のスキルを呼び出す機能だ。たとえばスキルの中で情報を紙にプリントアウトしたい時、EpsonやCanonのスキルとつなぐことで、印刷部分の実装コードは一切書かずに、自分の作ったスキルに印刷機能を加えることができる。@<br>{}
アメリカではこのSkill Connectionsを使って自分のスキルとUberによる送迎をつなげたり、レストランの予約機能を追加したりできる。日本では印刷機能が実装されている。



このSkill Connectionsはスキルの呼び出し元である「リクエスター」と呼び出し先である「プロバイダー」をマニフェストファイルにtasksというセクションを追加することで連携させる。呼び出した時Echoデバイスは「◯◯スキルを試してみますか？」と確認するが、これはAlexa内で生成されるメッセージで、リクエスター、プロバイダーのどちらのスキルにも実装されない。


==== Amazon Pay / スキル内課金


Amazon Payを使った課金スキル、またスキル内課金でもこのパターンが使われている。@<br>{}
Amazon Pay、スキル内課金ともに実際に決済する仕組みが必要となる。が、開発者はスキル内でその仕組みを実装する必要はない。Amazon Payであれば別のサーバーにてAmazon Payを使った決済システムを実装し、スキル内課金ではAlexaがそのシステムを持っている。スキルからはメッセージを通して課金処理を行うのだ。



//image[proactiveapi21][スキル内課金のフロー]{
//}




これらの課金システムでは課金そのものについてのメッセージ、たとえばAmazon PayであればPINコードの入力、スキル内課金であれば値段とその確認などはスキル内では実装せず、Alexaが代理で話す。その間セッションは切れていて、決済部分が完了した時点で再びスキルにプロセスが戻り、セッションも新たに始まる。  



Alexaスキルはさまざまな機能が単純な仕組みで実装できる代わりに、その連携部分をAlexaが担当することがあり、その間プロセスはスキルから切り離され、開発者からのコントロールは効かなくなる。この感覚を覚えておくと、今後Alexaに似たような機能が追加された時もセッション管理などで混乱することはなくなるだろう。


//clearpage



== おわりに


いかがだっただろうか。通知機能の取り回しの良さとAlexaのメッセージングの考え方が理解できただろうか。



通知機能はスキル側からユーザーにアプローチできる数少ない手段だ。ぜひ積極的に活用して、スキルの使い勝手をもう一段階上げてほしい。@<br>{}
また、ここ数年でAlexa ConversationsがリリースされるとVUIに関する実装方法が大きく変わるだろう。その際に他のスキルとの差をつけられるのはこのような外とのつながりがある総合的なソリューションになる。今のうちから準備をしておこう。


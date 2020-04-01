= Alexaスキル開発をより便利にする、AWS活用法 =

//image[tag][]{
//}

//flushright{
著者：Hidetaka Okamoto（@<href>{https://twitter.com/motchi0214/,@motchi0214}）
//}

== はじめに

Alexaスキル開発の中で、あまり意識されずに使われていることの多いAWS。  
コミュニティのイベントなどでも、「Alexaスキル開発で初めてAWSを触りました」という声を聞くことが少なくありません。

そこで本章では、あまり意識せずに使っていることの多いAmazon Web Services（以下AWS）について、より便利に使い倒す方法を紹介します。

===== 本章の対象読者@<br>{}

本章の対象読者は、次のような方です。  

 * Alexaスキルをすでに作ったことがある人
 * ASK-CLIやHosted Skillで開発しているが、AWSはあまり意識したことがない人
 * AWSは触っているが、Alexaスキル開発には使ったことがない人

== 事前の準備

本章では、AWSのリソースを使用し、その操作や情報の確認にAWS-CLIを使用します。 

そのため、事前に以下の２点を用意する必要があります。

 * AWSアカウント
 * AWS CLI

=== AWSアカウントの準備
本章ではAWSのリソースを操作します。そのため、あなたのアカウントをAWS上に作成する必要があります。  
作成の方法については、AWSがチュートリアルを用意しておりますので、そちらを参考にしながらトライしてみましょう。

 * [AWSアカウント作成の流れ: https://aws.amazon.com/jp/register-flow/]

クレジットカード情報を入力する必要がありますが、AWSサポートプランの契約や無料枠のないサービスまたは無料枠を超えたサービスの利用を行わない限りはカードに請求が行われることはありません。

==== AWSの無料枠
AWSの主要なサービスには、無料枠が設定されています。
例えばAlexaでよく利用するAWS Lambdaの場合、以下の範囲であれば無料で利用することができます。

 * 1,000,000 件/月の無料リクエスト
 * 1 か月あたり最大 320 万秒のコンピューティング時間  
参考: https://aws.amazon.com/jp/s/dm/landing-page/create-free-account/

ただしこの無料枠は、「12ヶ月のみ無料」と「無期限に無料」の2種類が存在します。
Alexaスキル開発の場合、Amazon S3やAmazon EC2などが「12ヶ月無料」のサービスとなりますのでご注意ください。

=== AWS CLIの準備
続いてAWS CLIをセットアップしましょう。

AWS CLIはPythonで動作するコマンドラインツールです。そのため、利用しているマシンに「Python 3 バージョン 3.4 以降」がインストールされている必要があります。

 * [AWS CLIのインストール方法: https://docs.aws.amazon.com/ja_jp/cli/latest/userguide/install-cliv1.html]


==== AWS CLIのバージョンについて
本章では、以下のバージョンを利用しています。

//cmd{
$ aws --version
aws-cli/1.16.309
//}

これよりも古いバージョンを利用している場合や2019年12月時点でプレビュー版が公開されているAWS CLI version2などを利用した場合は、記載している内容とは異なる結果となる場合がありますのでご了承ください。

//note[Note: AWS CLIを覚えるメリット]{
本章ではAWSのコンソール上の操作方法について、なるべくAWS CLIを使用します。
これは紹介する内容をできるだけ長い期間、多くの方に参考にしていただけるようにするためです。

AWSをはじめとするクラウドサービスでは、UIのアップデート速度が非常に早く、執筆中にUIが変更されてしまうということも少なくありません。
そのため、スクリーンショットを用いた画面操作の説明も行いますが、アップデートの影響を受けにくいAWS CLIでの操作についても紹介するようにしています。
//}

== AWSとAlexaの関係性

まずはじめに、AWSとAlexaの関係性について簡単に紹介します。

AlexaはAmazonが提供するサービスの１つで、音声をJSONに変換してバックエンドに送信し、レスポンスのJSONを音声に変換して再生するという動きを行います。
バックエンドにはAWS Lambdaを指定することができますが、HTTPSのAPIを指定することも可能です。
そのため、Alexaスキルを作る上でAWSは必須ではありません。HerokuやGoogle Cloudなど、HTTPSのAPIを作成できる環境があれば、自分の手に馴染んだ環境を選択することができます。

しかしAlexaのバックエンドとしてよく選ばれるのはAWSの提供するAWS Lambdaです。これにはいくつか理由が考えられます。

まず1つはASK CLIの存在です。Alexaスキルを開発するためのCLIツールであるASK CLIはバックエンドコードをAWS Lambdaにデプロイします。そのためASK CLIを利用してバックエンドのコードを管理する場合、ほぼ間違いなくAWS Lambdaを利用することとなります。

2つ目の理由としては、Alexa / AWS双方がAlexaとAWS Lambdaを組み合わせて利用することを前提としたドキュメントやサンプルを用意していることです。
通常のHTTPS APIでは、Alexaから来たリクエストであるかどうかを判定する処理を自前で実装する必要があります。しかしAWS Lambdaの場合、コンソールでスキルIDを設定するだけで判定が可能となります。
また、本章後半にて紹介しますが、Alexaスキル開発者のためのクーポン提供プログラムなどもAWSが用意しており、特別な理由がない限りはAWSをAlexaのバックエンドに使用することが無難であるといえます。

//note[Note: AWSが提供するAlexa: Alexa for Business]{
AlexaはAmazonが提供するサービスです。そのため、AWSは「AlexaのバックエンドとしてのAWS」または「Alexaに対応したIoTデバイスを開発するためのAWS」のような形でAlexaについて言及します。
しかし実はAWSが主体となって提供しているサービスも存在します。

Alexa for BusinessはAWSが提供するAlexaのサービスです。通常のAlexaは家庭内での利用が想定された作りになっていますが、Alexa for Businessはビジネスでの利用を目的としています。
AWSのコンソール上でAlexaデバイスの管理を行うことや、会議室の予約・スケジュール管理・社内限定スキルの提供といった機能が利用できます。

残念ながら日本ではまだサービスを開始していませんが、今後のAWSの発表に期待したいところです。
//}

== Alexaスキル開発に欠かせない、基本サービス紹介
ここからはいよいよAlexaスキル開発の際にお世話になるAWSのサービスを紹介します。

=== AWS IAM

AWSを利用するにあたってまずはじめに触れる必要があるサービス。それは「AWS IAM」です。

IAMは「Identity and Access Management」を省略したもので、AWSのサービスやリソースに対するアクセスを管理するサービスです。
このサービスを用いることで、あなたのAWSアカウントをより安全に運用することができるようになります。

==== IAMユーザーのススメ

AWSアカウントを作成した後、管理画面やCLIへのアクセスに「作成したときのユーザー」を利用していませんか？
AWSでは、アカウントを作成するときに作成したユーザーのことを「ルートユーザー」とよびます。
このルートユーザーは、請求決済画面も含めたAWS上のすべてのサービスにアクセスができます。
そのため万が一このルートユーザーの認証情報などが漏れてしまうと、AWSアカウントを乗っ取られる恐れが発生します。

AWSでは、日常的な管理画面へのアクセスやCLI / API操作について、専用のIAMユーザーを作成することを推奨しています。
例えば以下のようなユーザーをそれぞれ作成します。

//emlist[IAMユーザーの例]{

ユーザー名：userForAdmin
種別　　　：管理画面アクセス用ユーザー
用途　　　：ルートに近い権限を持ち、MFAを有効にする

ユーザー名：forAWSCLI
種別　　　：CLI操作用ユーザー
用途　　　：ルートに近い権限を持つが、管理画面にはログインできない。アクセスキー・シークレットキーは定期的に更新する

ユーザー名：johnAlexaDev
種別　　　：Alexaスキル開発者用ユーザー（John）
用途　　　：開発者用個別アカウント。プログラムからのアクセスのみ。退職や異動時にユーザー削除

ユーザー名：janeAlexaDev
種別　　　：Alexaスキル開発者用ユーザー（Jane
用途　　　：開発者用個別アカウント。プログラムからのアクセスのみ。退職や異動時にユーザー削除

//}

このようにユーザーを細かく作成することで、万が一Gitに認証情報をコミットしてpushしてしまった場合にも、被害を最小限にすることができます。

==== ユーザー・ポリシー・ロール

AWS IAMには、「ユーザー」「ポリシー」「ロール」という機能があります。
他にもIDプロバイダーやグループといった機能もありますが、まずはこの３つを理解しましょう。

===== IAMユーザー：誰がAWSアカウントにアクセスしようとしているかを管理する@<br>{}

IAMユーザーは、AWSアカウントにアクセスするユーザーです。AWS CLIやASK CLI、ブラウザ・APIからアクセスするために必要です。
IAMユーザーはAWSアカウントに2つの方法でアクセスできます。

１つはAPIやCLIなどの「プログラムによるアクセス」です。これはアクセスキーIDとシークレットアクセスキーの２つを発行し、この認証情報を用いてAWSにアクセスします。
「パブリックなリポジトリにアクセスキーをpushして不正アクセスされた」のような話は、おおよその場合このプログラムからのアクセスのために発行した認証情報をコードにベタ打ちしてそのままコミットしたために発生しています。

もう１つの方法は、ブラウザからアクセスする「AWSマネジメントコンソールへのアクセス」です。こちらはよくあるwebサービスのようにユーザー名とパスワードを認証情報に利用します。
こちらについては，後からMFAを有効にすることなども可能です。

//listnum[iamgetuser][IAM user][bash]{
$ aws iam get-user --user-name hide-ipad
{
    "User": {
        "UserName": "hide-ipad", 
        "Tags": [
            {
                "Value": "hide-ipad", 
                "Key": "Name"
            }
        ], 
        "PasswordLastUsed": "2019-02-03T12:50:59Z", 
        "CreateDate": "2019-02-03T12:48:02Z", 
        "UserId": "XXXXXXXXXXXXXX", 
        "Path": "/", 
        "Arn": "arn:aws:iam::9999999999:user/hide-ipad"
    }
}
//}

===== IAMポリシー：AWSアカウントでどの操作をしてよいかを管理する@<br>{}

先程のIAMユーザーは、AWSアカウントにアクセスするユーザーについてのみ管理しています。
そのため、ユーザーを作っただけでは、ログインはできてもAWS上のサービスにアクセスすることができません。

AWSサービスへのアクセスを管理する仕組みは、IAMポリシーとよばれています。
IAMポリシーには、「AWSによって管理」されているものと「ユーザーによる管理」という自身で用意するものが存在します。

以下は「AWSによって管理」されている、AWS Lambdaを実行するために必要なポリシーの内容です。

//listnum[iampolicyexample][AWS Lambda Excute policy][bash]{
$ aws iam get-policy-version --version-id v1 --policy-arn arn:aws:iam::aws:policy/AWSLambdaExecute
{
    "PolicyVersion": {
        "CreateDate": "2015-02-06T18:40:46Z", 
        "VersionId": "v1", 
        "Document": {
            "Version": "2012-10-17", 
            "Statement": [
                {
                    "Action": [
                        "logs:*"
                    ], 
                    "Resource": "arn:aws:logs:*:*:*", 
                    "Effect": "Allow"
                }, 
                {
                    "Action": [
                        "s3:GetObject", 
                        "s3:PutObject"
                    ], 
                    "Resource": "arn:aws:s3:::*", 
                    "Effect": "Allow"
                }
            ]
        }, 
        "IsDefaultVersion": true
    }
}
//}

基本的には「AWSによって管理」の中にあるポリシーを使いつつ、必要に応じてカスタムしていくという運用イメージです。
そしてAlexaスキルや特定のアプリケーションで、特定のリソース・アクションの操作だけを許可したいという場合には、以下のような形でポリシーを作成します。

//listnum[iamcreatepolicy][Create policy][bash]{
$ aws iam create-policy --policy-name demo --policy-document '{
  "Version": "2012-10-17", 
  "Statement": [
      {
          "Action": [
            "s3:PutObject",
            "s3:GetObject",
            "s3:DeleteObject"
          ],
          "Resource": [
            "arn:aws:s3:::my-bucket-for-alexa/*"
          ],
          "Effect": "Allow"
      }, 
      {
          "Action": [
            "dynamodb:PutItem",
            "dynamodb:DeleteItem",
            "dynamodb:GetItem"
          ],
          "Resource": [
            "arn:aws:dynamodb:us-east-1:*:table/MyAlexaDB"
          ], 
          "Effect": "Allow"
      }
  ]
}'
//}

この例では、以下の操作が許可されています。

//emlist[ポリシーの例]{

リソース　　：Amazon DynamoDB(MyAlexaDBテーブル)
許可 / 拒否：許可
操作内容　　：データのPUT / GET / DELETE

リソース　　：Amazon S3(my-bucket-for-alexaバケット)
許可 / 拒否：許可
操作内容　　：ファイル（オブジェクト）のPUT / GET / DELETE

//}

この設定であれば、誤ってテーブルやバケット本体を削除してしまうことや、全く関係のないリソースへアクセスするといったことも防ぐことができます。

//note[Note: FullAccessにはご用心]{
「AWSによって管理」のポリシーには、「（サービス名）FullAccess」というものが多く存在します。
これを使うと簡単にそのサービスへのアクセスができるようになります。ですが、本番環境でこのポリシーを利用することはあまりおすすめできません。

このポリシーは、「そのサービスすべて」へのアクセスを許可します。そのため、DBやサーバーなどのリソースそのものを削除することや、アクセス権限を変更してしまうということも可能となります。
そのためめったに起きることではないとは思いますが、typoやケアレスミスによって突然データやサーバーがなくなってしまう・アクセスできなくなるということもあり得る状況と言えます。

初めてそのサービスにふれるときには、FullAccessでざっと理解するという方法が効率的です。ですが、本番環境に適用する際は、なるべく最小限の操作・リソースアクセスのみを許可したポリシーをつくるようにしましょう。
//}

===== IAMロール：ポリシーをまとめて役割（ロール）にする@<br>{}

IAMポリシーでAWSのリソースアクセスを設定できることがわかりました。ということは、作成したIAMユーザーにIAMポリシーを設定してやれば、それでAWSへのアクセス管理ができるようになります。
ですが、AWSのリソースから他のリソースにアクセスしたい場合、ここに「IAMロール」という機能も必要となります。

IAMロールは、作成したポリシーをグループ化して役割（ロール）として定義できるという仕組みです。
そしてAWSのリソース、例えばAWS LambdaがAmazon DynamoDBにアクセスしたい場合などには、AWS Lambdaがどのような役割をもって実行しているかを設定する必要があります。
この仕組みのことを「実行ロール」とよびます。

AWS Lambdaを用いてAmazon DynamoDBやAmazon S3などのリソースにアクセスしたい場合、この実行ロールに適切なポリシーを割り当ててやることが重要です。

=== ASK CLIが作成するAWS Lambdaのロール

ASK CLIでスキルをデプロイする場合、ASK CLIが作成するLambda関数には以下のポリシーを持つロールが設定されています。

//listnum[askcliDefaultRole]{
{
    "PolicyVersion": {
        "CreateDate": "2015-04-09T15:03:43Z", 
        "VersionId": "v1", 
        "Document": {
            "Version": "2012-10-17", 
            "Statement": [
                {
                    "Action": [
                        "logs:CreateLogGroup", 
                        "logs:CreateLogStream", 
                        "logs:PutLogEvents"
                    ], 
                    "Resource": "*", 
                    "Effect": "Allow"
                }
            ]
        }, 
        "IsDefaultVersion": true
    }
}
//}

これは、Lambda関数を実行した際のログをCloudWatch Logsに書き込むために必要な権限のみ許可されている状態です。

そのため、persistantAttributesを利用してAmazon DynamoDBやAmazon S3にアクセスしたい場合、ここにその権限を追加してやる必要があります。

==== Alexaスキル用Lambdaのポリシーを用意しよう

理想的な運用はスキルやアプリケーションごとにIAMポリシーを作成していくことです。
ですが、個人開発やテスト的な開発を目的としている場合には、都度IAMポリシーを作成することはあまり効率的ではありません。

そこでまずおすすめしたいのは、「Alexaスキル用のIAMポリシー」を１つ用意することです。
以下のコマンドは、Amazon DynamoDBとAmazon S3に対してASK SDKが行う操作をまとめたものです。

//listnum[iamcreatepolicyExample][Create policy][bash]{
$ aws iam create-policy --policy-name forAlexaSkillLambdaFunction --policy-document '{
  "Version": "2012-10-17", 
  "Statement": [
      {
          "Action": [
            "s3:PutObject",
            "s3:GetObject",
            "s3:DeleteObject"
          ],
          "Resource": [
            "arn:aws:s3:::*"
          ],
          "Effect": "Allow"
      }, 
      {
          "Action": [
            "dynamodb:CreateTable",
            "dynamodb:PutItem",
            "dynamodb:DeleteItem",
            "dynamodb:GetItem"
          ],
          "Resource": [
            "arn:aws:dynamodb:*:*:table/*"
          ], 
          "Effect": "Allow"
      }
  ]
}'
//}

ASK CLIなどで作成したLambda関数の実行ロールに、このポリシーを追加してやることで、Amazon DynamoDBとAmazon S3へのアクセスを可能にすることができます。

より厳密に行いたい場合は、以下のようにAmazon DynamoDBとAmazon S3へのアクセスポリシーを個別に作成するとよいでしょう。

//listnum[iamcreatepolicyExample2][Create policy][bash]{
$ aws iam create-policy --policy-name AlexaS3AccessPolicy --policy-document '{
  "Version": "2012-10-17", 
  "Statement": [
      {
          "Action": [
            "s3:PutObject",
            "s3:GetObject",
            "s3:DeleteObject"
          ],
          "Resource": [
            "arn:aws:s3:::*"
          ],
          "Effect": "Allow"
      }
  ]
}'

$ aws iam create-policy --policy-name AlexaDynamoDBAccessPolicy --policy-document '{
  "Version": "2012-10-17", 
  "Statement": [
      {
          "Action": [
            "dynamodb:CreateTable",
            "dynamodb:PutItem",
            "dynamodb:DeleteItem",
            "dynamodb:GetItem"
          ],
          "Resource": [
            "arn:aws:dynamodb:*:*:table/*"
          ], 
          "Effect": "Allow"
      }
  ]
}'
//}

//note[Note: Serverless Frameworkでより簡単に権限管理]{
本章後半で紹介する「Serverless Framework」というライブラリを使用することで、このIAM管理をより簡単に行うことができます。

//listnum[slsExample]{
service:
  name: alexa-skill-example

provider:
  name: aws
  runtime: nodejs12.x
  logRetentionInDays: 14
  region: us-east-1
  stage: development
  iamRoleStatements:
    - Effect: "Allow"
      Action:
        - "s3:PutObject"
        - "s3:GetObject"
        - "s3:DeleteObject"
      Resource: 
        - "arn:aws:s3:::*"
    - Effect: "Allow"
      Action:
        - "dynamodb:CreateTable",
        - "dynamodb:PutItem",
        - "dynamodb:DeleteItem",
        - "dynamodb:GetItem"
      Resource:
        - "arn:aws:dynamodb:*:*:table/*"

functions:
  skillBackend1:
    handler: index.handler
    events:
      - alexaSkill
  skillBackend2:
    handler: index.handler2
    events:
      - alexaSkill
//}

Serverless Frameworkの場合、同一のテンプレート内に定義されたLambda関数はすべて同じ実行ロールが設定されます。
Lambda関数を複数管理したい場合には、こちらを使う場合が便利になることもあります。
//}

==== AWS IAMまとめ

 * AWS IAMでAWSのリソースに対するアクセスを管理できる
 * ルートユーザーは使わず、ログイン用・CLI用それぞれでIAMユーザーを作ろう
 * AWS Lambdaを他リソースに連携するには実行ロールにIAMポリシーを追加する必要がある
 * 必要最低限のポリシーを設定して、より安全にAWSを利用しよう

=== AWS Lambda

続いてはAlexaスキルのバックエンドとして欠かせない存在であるAWS Lambdaについて紹介します。

==== AWS Lambdaの特徴

AWS Lambdaは他のAWSサービスと同様に従量課金制のサービスです。
Lambda関数が呼び出された回数と、その処理の実行時間に応じて課金される仕組みで、1 か月に 1,000,000 件のリクエストと 400,000 GB-秒の実行時間について無料枠が用意されています。
また、いわゆるサーバーレスなサービスの１つですので、アクセスが集中した場合のスケーリングやインフラレベルの更新などについてはAWS側がすべて管理してくれます。

そのためAWS Lambdaを使う場合は、「指定した実行環境（Nodejs12.x / Python3など）で、動くコードを実装してデプロイするだけ」で実装したコードをAWSクラウド上で実行できるようになります。

Alexaスキルでは、このAWS LambdaでAlexaが聞いた内容を処理して返答を作成します。
デプロイや関数の作成についても、基本的にはASK CLIやServerless Frameworkといったツールを経由することになります。

==== RuntimeのEOL(End of Life)に要注意

AWS Lambdaを利用する中で注意しないといけない点の１つが実行環境（Runtime）のサポート期限についてです。
各言語にはアップデートに伴いサポートを終了する(End of Life)バージョンが発生します。
サポートが終了したバージョンについては、セキュリティアップデートなどの重要な更新が行われなくなり、安全に動作しなくなるリスクが高くなる恐れがあります。

AWSでは、「ランタイムサポートポリシー@<fn>{runtimePolicy}」というドキュメントでサポートの期限について紹介しています。
パッケージは本章のサポートサイトから入手できます。
各自ダウンロードしてインストールしておいてください。
//footnote[runtimePolicy][AWS ランタイムサポートポリシー: https://docs.aws.amazon.com/ja_jp/lambda/latest/dg/runtime-support-policy.html ]

一部の言語・バージョンについては、公式がEOLになってからもしばらくの間関数を更新することができます。
が、新しいLambda関数を作成することはできなくなりますので、なるべく早い段階でバージョンを更新することをおすすめします。

Alexaスキル開発で用いられやすいNode.jsは、このEOLの周期が比較的短いという特徴があります。
2019年だけでも4.3(4/30) / 6.10(8/12)の2バージョンが廃止され、2020年2月には8.10についても廃止予定です。

ASK CLIや手動でLambda関数を作成した場合、このランタイムのバージョン更新を手動で行う必要があります。
スキルの数に比例して更新作業量が増えることになりますので、CloudFormationやServerless Framework / terraformといったコードでリソースを管理できるツールを導入することをおすすめします。

もし作成済みのLambda関数のバージョンについて把握できていないという方は、「lambda-function-search」というCLIツールをnpm上に公開していますので、こちらを使って調べてみるとよいでしょう。

//cmd{
$ npx lambda-function-search --runtime nodejs8.10 --region all
npx: 54個のパッケージを21.781秒でインストールしました。
Search condition: Runtime === nodejs8.10
=== Matched Functions: 0 / 13 ===
Region : ap-northeast-1
=== Matched Functions: 0 / 13 ===
Region : ap-northeast-2
=== Matched Functions: 0 / 13 ===
=== Matched Functions: 79 / 177 ===
Region : us-east-1
ask-custom-battle-of-math-default
ask-custom-wp-meetup-finder-default
...
//}

==== ASK CLIを使ってデプロイ済のAWS Lambdaをスキルに設定する

Alexaスキルを複数開発していくと、ASK CLIで作成されるLambda関数ではなく自前で用意したLambda関数をスキルに紐づけたくなることがあります。
その場合は、ASK CLIで作成したディレクトリ内にある「skill.json」の設定を変更します。

//emlist[skill.jsonの書き換え箇所抜粋]{

// ASK CLIが作成したJSON
{
  "manifest": {
    ...
    "apis": {
      "custom": {
        "endpoint": {
          "sourceDir": "lambda/custom",
          "uri": "ask-custom-wp-meetup-finder-default"
        }
      }
    }
  }
}


// 任意のLambda関数を指定する場合
{
  "manifest": {
    "apis": {
      "custom": {
        "endpoint": {
          "uri": "arn:aws:lambda:us-east-1:9999:function:YOUR_SKILL_ARN"
        },
        "interfaces": []
      }
    }
  }
}
//}

すでにASK CLIでLambda関数をデプロイしている場合には、「.ask/config」の以下のブロックを削除する必要があります。

//listnum[askConfigJSON][削除が必要なブロック]{

{
  "deploy_settings": {
    "default": {
      "skill_id": "amzn1.ask.skill.9999-99999-99999-99999",
      "was_cloned": false,
      "merge": {},
      "resources": {
        "manifest": {
          "eTag": "b4792c6b558861336ebee442989c74e5"
        },
        "interactionModel": {
          "ja-JP": {
            "eTag": "d8d5ca22935ea3dbbc565accfcd72f68"
          }
        },
        // ここから削除
        "lambda": [
          {
            "alexaUsage": [
              "custom/default"
            ],
            "arn": "arn:aws:lambda:us-east-1:9999:function:YOUR_SKILL_ARN",
            "awsRegion": "us-east-1",
            "codeUri": "lambda/custom",
            "functionName": "your-skill-function-name",
            "handler": "index.handler",
            "revisionId": "ea710830-2250-47b9-b90b-ca2f0617fc16",
            "runtime": "nodejs8.10"
          }
        ]
        // ここまで削除
      }
    }
  }
}

//}


//note[Note:ASK CLIでデプロイしたLambda関数の情報を取得する]{

AWS-CLIでLambda関数の設定を変更するためには、ASK-CLIがデプロイしたLambda関数の情報を取得する必要があります。

以下のコマンドを実行することで、簡単に関数名やARNを取得することができます。

//cmd{
$ cat .ask/config | jq .deploy_settings.default.resources.lambda
[
  {
    "alexaUsage": [
      "custom/default"
    ],
    "arn": "arn:aws:lambda:us-east-1:9999999:function:ask-custom-your-function-default,
    "awsRegion": "us-east-1",
    "codeUri": "lambda/custom",
    "functionName": "ask-custom-your-function-default,
    "handler": "index.handler",
    "revisionId": "xxx-xxx-xxx-xxxx,
    "runtime": "nodejs12.x"
  }
]
//}

//}

==== ColdStartとその対策

AWS Lambdaは、関数が呼び出される度に実行環境がセットアップされるという仕組みです。
そのため、Lambda関数を実行するために必要なメモリ容量や実行時間の設定、プロビジョニングといったブートストラップの時間だけレスポンスが遅れることがあります。

AWSのドキュメントには以下のような説明があります。

//quote{
実行コンテキストの設定には時間がかかります。また、「ブートストラップ」が必要なため、Lambda 関数を呼び出すたびに若干のレイテンシーが発生します。
通常、この遅延は Lambda 関数を初めて呼び出したとき、または更新されたときに発生します。
Lambda 関数を連続して呼び出す場合、AWS Lambda はできる限り実行コンテキストを再利用します。

AWS Lambda > 開発者ガイド > AWS Lambda 実行コンテキスト
https://docs.aws.amazon.com/ja_jp/lambda/latest/dg/running-lambda-code.html
//}

設定やコードのサイズによりますが、Alexaスキルの場合おおよそ0.5 ~ 1秒程度のレイテンシーが発生するといわれています。

===== 対策1: CloudWatch Eventなどでwarmupする@<br>{}

コールドスタートによる遅延を減らす方法としてよく用いられるのが、CloudWatch Eventなどで定期的にLambda関数を起動させ続ける方法です。
CloudWatch Eventによって定期的にLambda関数が起動されるため、コールドスタートの可能性を減らすことが期待できます。

//cmd[AWS CLIによるwarmupの設定]{

// CloudWatch Eventを作成
$ aws events put-rule --name "LambdaWarmup" --schedule-expression "rate(5 minutes)" 
{
    "RuleArn": "arn:aws:events:us-east-1:xxxxxxxxxxxx:rule/LambdaWarmup"
}

// CloudWatchからのアクセスを許可
$ aws lambda add-permission \
  --function-name my-alexa-function \
  --statement-id "lambda-warmup" \
  --action lambda:InvokeFunction \
  --principal events.amazonaws.com \
  --source-arn arn:aws:events:us-east-1:xxxxxxxxxxxx:rule/LambdaWarmup

// CloudWatch EventをLambdaに設定する
$ aws events put-targets \
  --rule LambdaWarmup \
  --targets Arn=arn:aws:lambda:us-east-1:xxxxxxxxxxxx:function:my-alexa-function,Id=1

//}

この場合、Lambda関数に送られる値がAlexaから来るものと異なります。
ASK SDKで実行した関数は、CloudWatch Eventからの値を想定していない実装になることが多いため、予期せぬエラーが記録される可能性があります。

エラーを出さずに終了させるためには、以下のような実装を追加することをおすすめします。

//listnum[ignorecwe][CloudWatch Eventからの呼び出しを除外する]{

import { SkillBuilders } from 'ask-sdk'
export const handler = (event, context) => {
  if (event.source && event.source === "aws.events") return event
  return SkillBuilders.standard()
    .addRequestHandlers(...)
    .create().invoke(event, context)
}
//}

===== 対策2: Provisionend Concurrencyを設定する@<br>{}

AWS re:invent 2019にて発表された「Provisioned Concurrency」という機能を用いることでもコールドスタートを回避できます。
この機能は、設定した数の関数実行環境（コンテナ）を事前にプロビジョニングしておくことができます。
そのため、設定値を超えない限りはプロビジョニング済のLambda関数を実行することができるため、コールドスタートによる遅延を減らすことができます。

//cmd[AWS CLIでの設定方法]{
// バージョンを設定する必要があるため、設定する
$ aws lambda publish-version \
  --function-name my-alexa-function
{
  ...
  "Version": "1"
}

// 今後のアップデートに備えてエイリアス化する
$ aws lambda create-alias \
  --function-version 1 \
  --name production \
  --function-name my-alexa-function

// --function-name: 関数名
// --qualifier: 設定するバージョンまたはエイリアス
// --provisioned-concurrent-executions: プロビジョンする同時実行数
$ aws lambda put-provisioned-concurrency-config \
  --function-name my-alexa-function \
  --qualifier production \
  --provisioned-concurrent-executions 10

//}

Alexaスキルであれば、はじめは同時実行数1で設定し、様子を見ながら数を増やすという運用でもよいかもしれません。
キャンペーンなどでユーザー数が増加する可能性がある場合は数字を増やすという方法も考えることができます。

Provisioned Concurrencyのデメリットとしては、AWS Lambdaの無料枠の適用範囲外になることと、追加の利用料金が発生することによるコストの増加です。
クラスメソッドのブログにて費用について考察されている記事 @<fn>{priceOfProvision} がありますが、EC2やFargateを利用する場合と変わらない利用料金になるケースも存在する様子です。

//footnote[priceOfProvision][安い？それとも高い？Provisioned Concurrencyを有効化したLambdaのコストに関する考察： https://dev.classmethod.jp/cloud/aws/simulate-provisioned-concurrency-cost/]

===== 対策3: AWS Lambdaをやめる@<br>{}

最後の方法は、そもそもAWS Lambdaを使用しないという選択です。
Node.jsの場合、ExpressでASK SDKを実行するライブラリが用意されています。
そのため、Amazon EC2やAWS FargateなどにExpressでNode.jsのサーバーを立て、そこにAlexaスキルバックエンドを構築するという実装も可能です。

ask-sdk-express-adapter
https://www.npmjs.com/package/ask-sdk-express-adapter

//listnum[askExpress][ExpressでのAlexaスキル実装]{

import { SkillBuilders, StandardSkillBuilder } from 'ask-sdk'
import express from 'express'
import { ExpressAdapter } from 'ask-sdk-express-adapter'

const skill = SkillBuilders.standard()
                .addRequestHandlers(...)
                .create();

const adapter = new ExpressAdapter(skill, true, true);

const app = express();
app.post('/', adapter.getRequestHandlers());
app.listen(3000);

//}

こちらであればサーバーが常時起動しているため、コールドスタートでの遅延を心配する必要がなくなります。
もちろんサーバーが常時起動しているため、コストがAWS Lambdaよりも高くなる可能性があることに注意しましょう。

//note[Note: AWS Lambdaのほうが割高になるケース]{
AWS Lambdaは実行回数と実行時間に応じた従量課金のため、スキルの数や実行回数の増加に比例してコストが増加します。
しかしAWS FargateやAmazon EC2であれば1時間あたりのCPUやメモリに応じた課金です。
そのため1時間あたりの実行数が非常に多い場合やProvisioned Concurrencyなどのオプションの設定によっては、AWS Lambdaのほうが請求金額が大きくなる可能性もあります。

ざっくりとAWS FargateとAWS Lambdaの料金を試算したところ、月間の合計リクエスト数が7,500,000を超えたころからAWS Fargateの費用に近いコストになる様子でした。

//listnum[cost][料金試算]{
Fargateの最小費用（30日起動） 
vCPU : 0.25(vCPU) × 0.05056(USD) × 30(Days) × 24(Hours) = 9.1008(USD)
メモリ: 0.50(GB)   × 0.05056(USD) × 30(Days) × 24(Hours) = 1.9908(USD)
合計: 9.1008 + 1.9908 = 11.0916(USD)

Lambdaの利用料金（平均実行時間を1秒と仮定）
合計コンピューティング (秒)    : 7,500,000(Request) × 1 (sec)       = 7,500,000 秒
合計コンピューティング (GB-秒) : 7,500,000(Request) × 128 MB ÷ 1024 = 937,500 GB-秒
請求対象のコンピューティング時間 : 937,500 GB-秒 – 400,000 GB-秒の無料利用枠 = 537,500 GB-秒
1ヶ月のコンピューティング料金  : 537,500 GB-秒 × 0.00001667 USD = 8.96 USD

請求対象のリクエスト : 7,500,000(Request) - 1,000,000(Request) = 6,500,000(Request)
1ヶ月のリクエスト料金: 0.2(USD) * 6,500,000(Request) ÷ 1,000,000 = 1.30 USD

合計請求金額: 8.96 USD + 1.30 USD = 10.26 USD
//}

スキルを複数開発されている方は、ぜひ定期的に請求内容をチェックして、費用対効果の高いサービスがどれなのかを見極めるようにしましょう。
//}

==== １つのLambda関数に複数のスキルをまとめる

あまり利用頻度・更新頻度の高くないスキルであれば、そもそも作成するLambda関数を１つにまとめてしまうという方法もあります。
TypeScript版のSDKの実装コードを確認する限りでは、以下のような実装にすることで１つのLambda関数から複数のスキルを実行できるようになります。

 * 1. リクエストの「event.context.System.application.applicationId」からスキルIDを取得
 * 2. スキルIDに応じてStandardSkillBuilderまたはCustomSkillBuilderを返す関数を実装
 * 3. 2の戻り値に対して「create」と「invoke」を実行した結果をreturnする

以下はサンプルのコードです。

//listnum[skillExample][複数のスキルを１つのLambdaにまとめる]{
import { SkillBuilders, StandardSkillBuilder } from 'ask-sdk'
import { LambdaHandler } from 'ask-sdk-core/dist/skill/factory/BaseSkillFactory'

const createSkill = (skillId: string): StandardSkillBuilder => {
  const skill = SkillBuilders.standard()
  switch(skillId) {
    case 'amzn1.ask.skill.skill-1':
      return skill.addRequestHandlers(...)
    case 'amzn1.ask.skill.skill-2':
      return skill.addRequestHandlers(...)
    default:
      throw new Error('Unsupportes Skill ID')
  }
}
const handler: LambdaHandler = (event, context) => {
  const skillId = event.context.System.application.applicationId
  const skill = createSkill(skillId)
  return skill.create().invoke(event, context)
}  
//}

この実装にすることで、ランタイムの更新が必要なLambda関数の数を減らすことができます。
また関数が呼び出される回数が増えるため、コールドスタートになる確率も減らすことが期待できます。

ただし複数のスキルのコードが同居することになるため、「あるスキルへの更新のデプロイが原因で他のスキルに障害が発生する」というトラブルが発生するリスクがありますので、その点には注意する必要があります。

==== まとめ

 * AWS Lambdaを用いることで、簡単にAlexaのバックエンドを用意できる
 * ASK-CLIを使うことでデプロイも簡単に行うことができる
 * コールドスタートによる遅延とランタイムのEOLに注意
 * 人気スキルなどでは、AWS Fargateなどのほうが低コストになる場合も

=== Amazon DynamoDB

ここからはAlexaでDBとして用いられることの多い、Amazon DynamoDBについて紹介します。

==== Amazon DynamoDBの特徴

Amazon DynamoDBはフルマネージドなNoSQLデータベースです。
AWSが管理するDBサービスのため、スケーリングやプロビジョニング・ソフトウェアの保守などを行う必要はありません。
また、ゲームのデータストアにも利用されるなど、高いパフォーマンスを持つことも特徴です。

一方でRDBのように複数のテーブルをJOINして検索するようなことはできないため、テーブルやデータの設計をNoSQLにあわせた形にする必要があります。

//note[Note: RDBをDBに使うことはできるのか？]{

AWS Lambdaを利用する場合、AWSが提供するRDBMSであるAmazon RDSを組み合わせるのはあまり推奨されていませんでした。
これはコネクションプーリング機構が使いづらく、RDSの最大同時接続数以上にLambda関数が起動すると接続エラーが発生しやすいことが原因でした。

ですが、2019年12月にRDSへ接続するためのプロキシーとしてAmazon RDS Proxy with AWS Lambdaが発表されました。

Using Amazon RDS Proxy with AWS Lambda
https://aws.amazon.com/jp/blogs/compute/using-amazon-rds-proxy-with-aws-lambda/

まだプレビュー版ではありますが、今後AlexaのバックエンドにRDBを使用するパターンも増えてくるのかもしれません。

//}

==== ASK SDKからDynamoDBテーブルを作成する

ASK SDKの場合、以下のような書き方をすることでスキル起動時にDynamoDBテーブルを作成できます。

//listnum[createDBByASK][ASK SDKでのテーブル作成]{
import { SkillBuilders } from 'ask-sdk'

export const handler = SkillBuilders.standard()
  .withTableName('ExampleTable')
  .withAutoCreateTable(true)
  .lambda()
//}

インデックスの作成などは、テーブルの作成後にも可能です。
AWS CLIなどから作成する方法も１つですが、特に理由がなければASK SDKから作成してしまうのが最も手軽です。

==== persistanceAttributesのデータ構造

Alexaでは、Amazon DynamoDBにASK SDKのPersistanceAttributesとしてアクセスします。

//listnum[examplePersistanceAtt][Persistance Attributeの例]{
const ExampleHandler = {
  canHandle() {
    return true
  },
  async handle(handletInput) {
    // DynamoDB.get
    const atts = await handletInput.attributesManager.getPersistentAttributes()

    const usedCount = atts.usedCount || 0

    // Putするオブジェクトを作成
    handletInput.attributesManager.setPersistentAttributes({
      ...atts,
      usedCount: usedCount + 1
    })

    // DynamoDB.put
    await handletInput.attributesManager.savePersistentAttributes()
  
    return handletInput.responseBuilder
      .speak(`こんにちは。これで${usedCount}回目の起動です。`)
      .getResponse()
  }
}
//}

この実装をAWS SDKに置き換えると、以下のような形となります。

//listnum[examplePersistancebyAWS][AWS SDKで実装した場合]{

import { getUserId } from 'ask-sdk'
import { DynamoDB } from 'aws-sdk'

const tableName = 'TableName'
const ExampleHandler = {
  canHandle() {
    return true
  },
  async handle(handletInput) {
    const client = new DynamoDB.DocumentClient()
    const userId = getUserId(handletInput.requestEnvelope)
    
    const {Item: atts} = await client.get({
      TableName: tableName,
      Key: {
        id: userId
      }
    }).promise()

    
    const usedCount = atts.usedCount || 0
    await client.put({
      Item: {
        id: userId,
        attributes: atts
      },
      TableName : tableName,
    }).promise()
    return handletInput.responseBuilder
      .speak(`こんにちは。これで${usedCount}回目の起動です。`)
      .getResponse()
  }
}

//}

リクエスト毎にテーブル名やキーを指定する必要があるため、ASK SDKを使うことでコードの量を少なくすることができます。

==== AWS SDKのススメ

ASK SDKを使うことでシンプルなコードになる一方、Amazon DynamoDBの機能を活かしきれていないというデメリットも存在します。
Amazon DynamoDBでは、RDBほど高機能ではありませんがデータを検索するScanとQueryアクションを利用できます。

次のサンプルでは、ユーザーのスコアを更新しつつ、ゲーム全体のスコア履歴の更新や取得を行っています。

//listnum[exampleQueryAWS][AWS SDKで実装した場合]{
import {getUserId, RequestHandler} from 'ask-sdk'
import { DynamoDB } from 'aws-sdk'
import moment from 'moment'

const tableName = 'TableName'
const ExampleHandler:RequestHandler = {
  canHandle() {
    return true
  },
  async handle(handletInput) {
    const client = new DynamoDB.DocumentClient()
    const userId = getUserId(handletInput.requestEnvelope)
    const score = getScore(handlerInput)
    
    const time = moment()
    const unixTime = time.unix()

    // 全体のスコア履歴をPUT
    await client.put({
      TableName: tableName,
      Item: {
        id: 'scoreHistory',
        recordName: `score#${time.format('YYYYMMDD')}#${unixTime}`,
        score
      }
    }).promise()
    
    // ユーザーのデータを取得
    const {Item: atts} = await client.get({
      TableName: tableName,
      Key: {
        id: userId,
        recordName: 'userScore'
      }
    }).promise()

    const newScore = atts.score + score

    // ユーザーのスコアを更新
    await client.put({
      TableName: tableName,
      Item: {
        ...atts,
        score: newScore
      }
    }).promise()

    // 全体のスコア履歴を取得
    const {Items: histories} = await client.query({
      TableName: this.tableName,
      KeyConditionExpression: 'id = :id AND begins_with(recordName, :recordName)',
      ExpressionAttributeValues: {
        ':id': 'scoreHistory',
        ':recordName': `score#${time.format('YYYYMMDD')}`
      }
    }).promise()
    
    return handletInput.responseBuilder
      .speak([
        `あなたのスコアは${score}で、合計${newScore}です。`,
        `今日プレイした人のスコアは、${histories.map(history => history.score).join('、')}です。`
      ].join(' '))
      .getResponse()
  }
}
//}

このようにAWS SDKを用いたデータ操作について覚えることで、より柔軟なデータの操作が可能となります。

==== TTLを利用したデータの時限削除

Amazon DynamoDBでは、保存したデータに有効期限を設定する「TTL(Time To Live)」機能が存在します。
この機能は指定したアイテムのキーに保存された数字（UNIXタイムスタンプ）をすぎるとデータを削除するというものです。

下のコマンドでは、作成済のテーブル「TTLExample」にたいして、ttlというキーに保存された値で有効期限を設定しています。

//cmd[DYnamoDB テーブルにTTLを設定]{

$ aws dynamodb update-time-to-live \
  --table-name TTLExample \
  --time-to-live-specification "Enabled=true, AttributeName=ttl"

{
    "TimeToLiveSpecification": {
        "AttributeName": "ttl", 
        "Enabled": true
    }
}

//}

この状態で、以下のようにデータをPUTすると、2週間を経過したタイミングでこのデータが自動的に削除されます。

//listnum[ttlExample][TTLを用いたデータの保存]{

import { getUserId } from 'ask-sdk'
import { DynamoDB } from 'aws-sdk'
import moment from 'moment'

const tableName = 'TableName'
const ExampleHandler = {
  canHandle() {
    return true
  },
  async handle(handletInput) {
    const client = new DynamoDB.DocumentClient()
    const userId = getUserId(handletInput.requestEnvelope)
    
    await client.put({
      Item: {
        id: userId,
        message: "秘密のメッセージ",
        ttl: moment().add(2, 'weeks').unix()
      },
      TableName : tableName,
    }).promise()
    return handletInput.responseBuilder
      .speak(`メッセージを保存しました。このデータは2週間で自動的に削除されます。`)
      .getResponse()
  }
}

//}

ゲームのランキングやハイスコアデータの管理や、センシティブなデータの一時保存といったユースケースでぜひお使いください。

==== バックアップとロールバック

Amazon DynamoDBには各テーブルのバックアップを作成する機能が用意されています。
次の例では、ExampleTableというテーブルのバックアップを作成し、そのデータを用いて新しくRestoredTableというテーブルを作成しています。

//cmd[AWS CLIでのバックアップ作成とリストア]{

// バックアップを作成
$ aws dynamodb create-backup --table-name ExampleTable --backup-name ExampleTable-Backup
{
    "BackupDetails": {
        "BackupCreationDateTime": 1577883078.922, 
        "BackupName": "ExampleTable-Backup", 
        "BackupStatus": "CREATING", 
        "BackupType": "USER", 
        "BackupSizeBytes": 396, 
        "BackupArn": "arn:aws:dynamodb:us-east-1:99999999:table/ExampleTable/backup/01577883078922-aee47411"
    }
}

// 作成したバックアップから新しくテーブルを作成
$ aws dynamodb restore-table-from-backup \
  --target-table-name RestoredTable \
  --backup-arn arn:aws:dynamodb:us-east-1:99999999:table/ExampleTable/backup/01577883078922-aee47411

//}

なお、同じ名前のテーブルを作成することはできません。
そのため新しいテーブルとして作成するか、古いテーブルを削除してからの作成となります。

===== 継続的バックアップとPoint In Time Recovery(PITR)@<br>{}

Amazon DynamoDBでは、1秒単位でデータをバックアップし、リカバリすることができます。
事前にテーブルの設定を更新する必要がありますが、これを有効にすることで誤ったデータの更新などが発生した場合にも簡単に復旧が可能となります。

//cmd[PITRの有効化]{

$ aws dynamodb update-continuous-backups \
  --table-name ExampleTable \
  --point-in-time-recovery-specification PointInTimeRecoveryEnabled=true
{
    "ContinuousBackupsDescription": {
        "PointInTimeRecoveryDescription": {
            "PointInTimeRecoveryStatus": "ENABLED", 
            "EarliestRestorableDateTime": 1577883615.0, 
            "LatestRestorableDateTime": 1577883615.0
        }, 
        "ContinuousBackupsStatus": "ENABLED"
    }
}

//}

「EarliestRestorableDateTime」から「LatestRestorableDateTime」までの期間であれば、1秒単位でテーブルを復旧することができます。
下の例では、「ExampleTable」の直近で作成されたバックアップから「ExampleTable-RestoredLatestPITR」というテーブルをリストアしています。

//cmd[PITRからのリストア]{

$ aws dynamodb restore-table-to-point-in-time \
  --source-table-name ExampleTable  \
  --target-table-name ExampleTable-RestoredLatestPITR \
  --use-latest-restorable-time
}

//}

このようにバックアップの作成とリストア方法を覚えておくことで、いざというときにも落ち着いて復旧させることができます。

==== まとめ

 * Amazon DynamoDBにデータを保存することができる
 * ASK SDKを使えば簡単にデータの出し入れが可能になる
 * AWS SDKを使うことでより複雑なデータの保存や検索も可能になる
 * バックアップを作成して、万が一に備えよう

=== Amazon S3

Amazon S3はAWSが提供するストレージサービスです。
Alexaスキルでは、スキルアイコンやAPLの画像、SSMLで再生する音楽やビデオスキルのためのメディアなどを保存することに用いられます。

==== ストレージクラスを使い分けよう

Amazon S3は、データの用途によって「ストレージクラス」を設定することでより安価に利用することができます。
通常のアップロードでは、「スタンダード」クラスが設定されます。
しかしアクセスの頻度が少ないファイルや万が一ロストしても影響の大きくないデータの場合、「スタンダード」以外のストレージクラスを利用することもできます。

===== 29日以内に削除するファイルまたは高頻度のアクセスがあるファイルは「スタンダード」
スタンダードクラスは頻繁にアクセスのある画像ファイルの保存に向いています。
また、他のストレージクラスは30日以上の最低保存期間がありますので、短い期間で削除される可能性の高いデータについてもスタンダードクラスが推奨されます。

===== 128KB未満の低アクセスファイルなら「インテリジェントな階層化」
「インテリジェントな階層化」クラスは、アクセスパターンが変化し、保存期間が長いデータに適用できます。
AWSがアクセス頻度に応じてデータを高頻度または低頻度に移動させるため、利用料金を手軽に節約することができます。

リリースしたばかりのスキルや、あまり使われない見込みの画像ファイルなどはこの「インテリジェントな階層化」クラスで保存することをおすすめします。
なお、このクラスは30日を最低ストレージ期間と設定されていますので、短期間で削除されるデータの保存にはおすすめできません。

===== 128KB以上でアクセス頻度は多くないデータは「標準IA」or「1ゾーンIA」
128KB以上の場合は、「標準IA」クラスを用いることが一般的です。
また通常Amazon S3では3つのデータセンター（アベイラビリティゾーン）にデータを冗長化させていますが、この冗長化も不要な場合は「1ゾーンIA」というクラスも選択できます。

この２つのクラスは、128KBを請求時の最小オブジェクトサイズとしていますので、128KBを下回るファイルについてはコストが嵩む恐れがあります。
またこちらのクラスも30日を最低ストレージ期間と設定されていますので、短期間で削除されるデータの保存にはおすすめできません。

===== データのバックアップに「Glacier」

過去のデータをバックアップとして保存する場合には、「Glacier」クラスがおすすめです。
このクラスに設定した場合、データを取得するのに数分から最大数時間かかるというデメリットがありますが、他のクラスよりも低い価格でデータを保存できます。

過去のデータや決済情報のような長期間の保存が必要なデータについては、一定期間を過ぎてからこのクラスに移動することをおすすめします。

===== アップロードしたファイルのストレージクラスを変更する

AWS CLIでは、ファイルをコピーする形を取ることでストレージクラスを変更できます。

//cmd{
$ aws s3api copy-object \
  --copy-source YOUR-BUCKET/images/skill-icon-518.png \
  --bucket YOUR-BUCKET --key images/skill-icon-518.png \
  --storage-class INTELLIGENT_TIERING
//}

アクセス頻度が高くなさそうなデータが見つかった場合には、ストレージクラスを切り替えてコストダウンを目指しましょう。

==== DBとしてのAmazon S3

Amazon S3といえば、画像や動画などのメディアファイルの保存に使うケースが一般的です。
が、AlexaではASK SDKが利用するデータベースとしてAmazon S3のバケットを利用することができます。

ask-sdk-dynamodb-persistence-adapter
https://www.npmjs.com/package/ask-sdk-dynamodb-persistence-adapter

以下のように、`withPersistenceAdapter`でS3 adapterを指定するだけで、データをAmazon S3に送ることができるようになります。

//listnum[S3をDBにする場合の設定サンプル]{
import { SkillBuilders } from 'ask-sdk'
import { S3PersistenceAdapter } from 'ask-sdk-s3-persistence-adapter'

exports.handler = SkillBuilders.custom()
    .addRequestHandlers(...)
    .withPersistenceAdapter(
      new S3PersistenceAdapter({  
        bucketName: 'YOUR_S3_BUCKET_NAME'
      })
    ) 
//}

===== スキルDB用S3バケットを１つに集約しよう

Amazon S3のバケットは、1アカウントに100までという制限が設定されています。
そのため、1スキルに1バケットを毎回作成していると、101スキル目以降はAWSに上限緩和の申請を行う必要がでてきます。

この対策として筆者が行っているのは、「すべてのスキルで1つのS3バケットを利用する」という方法です。
以下のサンプルコードのように、S3のAdapterはバケット名に加えてpathPrefixも指定できます。

//listnum[S3をDBにする場合の設定サンプル]{
import { SkillBuilders } from 'ask-sdk'
import { S3PersistenceAdapter } from 'ask-sdk-s3-persistence-adapter'

exports.handler = SkillBuilders.custom()
    .addRequestHandlers(...)
    .withPersistenceAdapter(
      new S3PersistenceAdapter({  
        bucketName: 'YOUR_S3_BUCKET_NAME',
        pathPrefix: 'MyFirstSkill'
      })
    ) 
//}

これを利用をすることで、「{S3バケット名}/{スキル名}/{User ID}」という形でデータを保存できるようになります。
スキル名でディレクトリを切る形にすることで、スキル外からデータを参照する必要が出た際にも比較的検索しやすい形にできます。

Amazon DynamoDBをAWS SDK経由で操作する方針にした場合、ASK SDKが提供する関数を使う必要がなくなります。
そこでASK SDKでの保存先をAmazon S3にすることで、ユーザー個別の設定や起動回数のようなクエリする必要のないデータをAmazon S3へ、
スコアや行動履歴のようなクエリ・集計する必要があるデータをAmazon DynamoDBへという振り分けが可能になります。

Amazon S3とAmazon DynamoDB、それぞれの機能や特徴を把握することで、より効率的・低コストでスキルのバックエンドを作ることができるようになります。

===== S3 SelectでS3オブジェクトに対してSQLを実行する

S3にデータを保存するメリットの１つが、このS3 Selectという機能です。
これはS3に保存されたJSONやCSVファイルに対してSQLでのクエリが可能になるサービスです。

以下のサンプルでは、S3に保存したSMAPIから取得したデプロイ済みAlexaスキル一覧にSQLを実行しています。
SQLのクエリは、「公開済みでないカスタムスキル」を検索しています。

//listnum[S3 SELECTでSkill情報をクエリするサンプル]{

import { S3 } from 'aws-sdk'

const client = new S3()
const bucketName = 'S3_BUCKET_NAME'
const keyName = 'skills.json'
const query = "SELECT * FROM s3object s WHERE s.stage != 'live' AND 'custom' in s.apis"

const queryS3Object = async () => {
  return new Promise(async (resolve, reject) => {
    const data = await client
      .selectObjectContent({
        Bucket: bucketName,
        Key: keyName,
        ExpressionType: 'SQL',
        Expression: query,
        InputSerialization: {
          JSON: {
              Type: 'Lines',
          },
          CompressionType: 'NONE',
        },
        OutputSerialization: {
          JSON: {
              RecordDelimiter: ','
          }
        }
      }).promise()
    
    // S3 SELECTはeventStreamでデータを返すので、配列変換をかける
    const eventStream = data.Payload;
    const records = []
    eventStream.on('data', (event) => {
      if (event.Records) {
        records.push(event.Records.Payload)
      }
    })
    .on('error', (err) => {
      reject(err)
    })
    .on('end', () => {
      let planetString = Buffer.concat(records).toString('utf8');
      planetString = planetString.replace(/\,$/, '');
      planetString = `[${planetString}]`;
      try {
        const planetData = JSON.parse(planetString);
        resolve(planetData);
      } catch (e) {
        reject(new Error(`Unable to convert S3 data to JSON object. S3 Select Query: ${query}`));
      }
    });
  })
}

queryS3Object()
  .then(data => {
    console.log(data)
  }).catch(e => {
    console.log(e)
  })

//}

S3 SELECTは事前のデータ処理が必要かつ、eventStreamの処理が必要になるなど、少し慣れが必要な部分もあります。
ですが使いこなせるようになると、行政が公開しているオープンデータのCSVやJSONをS3にインポートし、S3 SELECTでデータ検索するような機能も実装できます。

==== Amazon S3まとめ

 * 画像・メディア・JSON・CSVなどを保存できる
 * ファイルのアクセス頻度や保存期間に応じてストレージクラスを変更しよう
 * ASK SDKならS3をDBにもできる
 * S3 SELECTでより複雑な検索も可能

== Infrastructure as Codeで管理を効率化しよう

ここからは紹介したAWSのサービスをより効率的に管理すための方法について紹介します。

「Infrastructure as Code」とは、サーバーやデータベースのようなインフラリソースをコードで定義するという考え方です。
サーバーの設定などをコードで定義することによって、定義したとおりの設定のリソースを誰でも簡単に複数作成することができるようになります。
インフラといえばVMやミドルウェアについて指すこともありますが、本章ではAlexaスキルバックエンドに必要な「AWSリソース」をインフラとして扱います。

=== Alexaスキルでインフラをコード管理するメリット

個人的な感想ですが、Alexaスキルのバックエンドをコード化すると助かる点として以下の5つがあります。

 * Lambda関数の実行ロールの設定漏れやミスが起きにくい
 * Lambda関数のランタイム更新が一括で行える
 * 同じ環境を作りやすく、すぐにまとめて削除できる
 * 複数のサービスを連携させても、依存関係を忘れにくい
 * マネジメントコンソールのデザイン変更の影響を受けにくい

すべての設定をYAMLやTypeScriptなどで記述するため、リソースの構成や設定が確認しやすいという点は大きなメリットです。

もちろんCLIやマネジメントコンソールで操作するよりも覚えるべき事柄が増えるため、
初めてAWSを触るという方にはなかなかの難敵です。
ですが、ある程度AWSを使いこなしてくると、サービスの数や設定項目が膨大にあるため、最終的にはコードでの管理が必要となるでしょう。

ここからは、筆者がAlexaスキル開発で用いているツールである「Serverless Framework」について紹介します。

=== Serverless Frameworkとは

Serverless, Inc.が提供するOSSの管理ツールで、Lambda関数を非常に簡単にセットアップすることができます。

==== Serverless Frameworkを使うメリット

Serverless Frameworkのメリットは、簡単に始めることができることです。
いくつかのCLIコマンドと、YAMLの書き方をすこし覚えるだけでAlexaスキルに必要なリソースをデプロイ・管理できます。
そしてプラグイン機構が用意されているため、プラグインを入れてYAMLに設定を足すだけで様々な設定が簡単に行なえます。

また、クラウドベンダー公式のツールではないため、AWSに限らずGoogleやAzureなど様々なサービスを管理できるというメリットもあります。
更にServerless FrameworkはAWSへのデプロイ時、一度AWS CloudFormationのテンプレートに変換します。
そのため、仮にServerless FrameworkからAWS CloudFormationやAWS SAMなどに移行したくなったときにも、
生成されたテンプレートファイルを利用することで移行の手間を減らすことができます。

==== Serverless Frameworkを使うデメリット

AWSの場合、Lambda関数を中心に設計されています。
そのためAWS Lambda以外のサービスが中心となる構成の場合、気がつけばCloudFormationのYAMLを書いていたということになりがちです。

また、プラグイン機構をもつツールの宿命ですが、サードパーティのプラグインがメンテナンスされなくなった場合に
将来のバージョンアップで不具合が発生する恐れがあるというリスクも存在します。

そのため、プラグインについては提供元やメンテナンス状況について確認してから導入することをおすすめします。

==== Serverless Frameworkをインストールする

Serverless FrameworkはnpmでCLIツールをインストールして作業します。
インストール後は、「serverless」「slss」「sls」の3コマンドが利用できるようになります。
どのコマンドも動作は同じですので、使いやすさや覚えやすさで選ぶことをおすすめします。

//cmd{

$ npm install -g serverless

$ sls -v
Framework Core: 1.59.3
Plugin: 3.2.5
SDK: 2.2.1
Components Core: 1.1.2
Components CLI: 1.4.0

//}

==== プロジェクトをセットアップする

まずはプロジェクトをセットアップしましょう。
以下のように「create」コマンドを実行することで必要なファイルを作成してくれます。


//cmd[Lambda(Node.js)をセットアップする]{
$ sls create -t aws-nodejs-ecma-script -p my-first-lambda

Serverless: Generating boilerplate...
Serverless: Generating boilerplate in "/Users/develop/my-first-lambda"
 _______                             __
|   _   .-----.----.--.--.-----.----|  .-----.-----.-----.
|   |___|  -__|   _|  |  |  -__|   _|  |  -__|__ --|__ --|
|____   |_____|__|  \___/|_____|__| |__|_____|_____|_____|
|   |   |             The Serverless Application Framework
|       |                           serverless.com, v1.59.3
 -------'

Serverless: Successfully generated boilerplate for template: "aws-nodejs-ecma-script"

$ cd my-first-lambda
$ npm install

$ tree . -I node_modules
my-first-lambda
├── first.js
├── package.json
├── second.js
├── serverless.yml
└── webpack.config.js

0 directories, 6 files

//}

AWSの設定については、「serverless.yml」というファイルに定義されています。

//listnum[serverless.ymlサンプル]{

service:
  name: my-first-lambda
# app and org for use with dashboard.serverless.com
#app: your-app-name
#org: your-org-name

# Add the serverless-webpack plugin
plugins:
  - serverless-webpack

provider:
  name: aws
  runtime: nodejs12.x

functions:
  first:
    handler: first.hello
  second:
    handler: second.hello
    events:
      - http:
          method: get
          path: second

//}

まずはproviderのブロックを見てみましょう。
nameに「aws」と書かれていますので、AWSにリソースをデプロイすることがわかります。
また、runtimeに「nodejs12.x」と書かれていますので、このプロジェクトでデプロイされるLambda関数はすべてnodejs12.xで動作することも伺えます。

Lambda関数の定義については、「functions」ブロックを見ることで把握できます。
「first」と「second」はそれぞれLambda関数の名前です。「service.nameの値」+「provider.stageの値（デフォルトはdev）」+「functionsブロックで定義した名前」でLambda関数が作成されます。
今回のサンプルの場合、以下のLambda関数が作成されます。

 * first: my-first-lambda-dev-first
 * second: my-first-lambda-dev-second

functionsの各ブロックでは、handlerという値が定義されています。
これがその関数ではじめに呼び出されるファイルと関数の名前です。

firstの場合はfirst.jsのhello関数、secondの場合はsecond.jsのhello関数がはじめに実行されます。
それぞれのファイルを開くと、hello関数にLambdaの実装が書かれていることがわかります。

//cmd[first.jsとsecond.jsの中身]{

$ cat first.js
// eslint-disable-next-line import/prefer-default-export
export const hello = (event, context, callback) => {
  const p = new Promise(resolve => {
    resolve('success');
  });
  p.then(() =>
    callback(null, {
      message: 'Go Serverless Webpack (Ecma Script) v1.0! First module!',
      event,
    })
  ).catch(e => callback(e));
};

$ cat second.js 
// eslint-disable-next-line import/prefer-default-export
export const hello = (event, context, cb) => {
  const p = new Promise(resolve => {
    resolve('success');
  });
  const response = {
    statusCode: 200,
    body: JSON.stringify(
      {
        message: 'Go Serverless Webpack (Ecma Script) v1.0! Second module!',
        input: event,
      },
      null,
      2
    ),
  };
  p.then(() => cb(null, response)).catch(e => cb(e));
};

//}

また、secondのLambda関数には「events」という値が設定されています。
これはそのLambda関数を実行するイベントを定義するもので、「http」が指定されている場合にはAmazon API GatewayでAPIが作成されます。
今回の例では、pathがsecondでmethodがgetなので、secondというパスに対してGETのリクエストを受け付けることがわかります。

Alexaスキルで利用できるようにするには、このブロックに「alexaSkill」という値を設定します。

//listnum[Alexa向けserverless.ymlサンプル]{

service:
  name: my-first-lambda
# app and org for use with dashboard.serverless.com
#app: your-app-name
#org: your-org-name

# Add the serverless-webpack plugin
plugins:
  - serverless-webpack

provider:
  name: aws
  runtime: nodejs12.x

functions:
  first:
    handler: first.hello
    events:
      - alexaSkill: amzn1.ask.skill.xxx-xxx-xxx-xxx # スキルのIDがわかっている場合
      - alexaSkill # スキルのIDが不明な場合（デプロイ時にwarningがでます）
  second:
    handler: second.hello
    events:
      - http:
          method: get
          path: second

//}

これでAlexaスキルからfirstというLambda関数を呼び出せるようになりました。
なお、デプロイの前にfirst.jsの中身をASK SDKを用いたものに変更する必要がありますのでご注意ください。

//note[Note: TypeScript対応のプロジェクトをセットアップする]{

TypeScriptで実装したコードは、デプロイ前にJavaScriptにコンパイルする必要があります。
通常ではtscやwebpackなどのコマンドを利用しますが、Serverless Frameworkの場合、以下のコマンドでプロジェクトを作成することでデプロイ時にビルドを自動実行させることができます。

//cmd{

$ sls create -t aws-nodejs-typescript -p second
$ tree second
.
├── handler.ts
├── package.json
├── serverless.yml
├── tsconfig.json
├── vscode
│   └── launch.json
└── webpack.config.js

1 directory, 6 files

//}

TypeScriptでAWS Lambdaを実行するために最低限必要なライブラリや設定がされた状態でプロジェクトが起動します。
細かい設定については個別で行う必要がありますが、セットアップの手間をかなり簡略化できておすすめです。

//}

==== プロジェクトのデプロイと削除

設定が終われば、あとは実際にデプロイしてみましょう。
最後にデプロイしたリソースの情報が表示されればデプロイ完了です。

また「serverless-webpack」プラグインを利用していますので、ログをみるとデプロイの前にwebpackでのビルドが実行されていることがわかります。


//cmd[sls deploy]{

$ sls deploy
Serverless: Bundling with Webpack...
Time: 1818ms
Built at: 2020-01-02 4:14:54 PM
    Asset      Size  Chunks             Chunk Names
 first.js  4.33 KiB   first  [emitted]  first
second.js  4.42 KiB  second  [emitted]  second
Entrypoint first = first.js
Entrypoint second = second.js
[./first.js] 324 bytes {first} [built]
[./second.js] 411 bytes {second} [built]
Serverless: Packaging service...
Serverless: Warning! You are using an old syntax for alexaSkill which doesn't restrict the invocation solely to your skill. Please refer to the documentation for additional information.
Serverless: Uploading CloudFormation file to S3...
Serverless: Uploading artifacts...
Serverless: Uploading service my-first-lambda.zip file to S3 (3.22 KB)...
Serverless: Validating template...
Serverless: Updating Stack...
Serverless: Checking Stack update progress...
..................
Serverless: Stack update finished...
Service Information
service: my-first-lambda
stage: dev
region: us-east-1
stack: my-first-lambda-dev
resources: 16
api keys:
  None
endpoints:
  GET - https://xxxxxxx.execute-api.us-east-1.amazonaws.com/dev/second
functions:
  first: my-first-lambda-dev-first
  second: my-first-lambda-dev-second
layers:
  None
Serverless: Run the "serverless" command to setup monitoring, troubleshooting and testing.

//}

endpointsに表示されているURLに実際にリクエストを投げてみると、second.jsのhello関数の実行結果が返ってくることがわかります。


//cmd[デプロイされたAPIを呼び出してみる]{

$ curl https://xxxxxxx.execute-api.us-east-1.amazonaws.com/dev/second | jq .
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100  3170  100  3170    0     0   3457      0 --:--:-- --:--:-- --:--:--  3456
{
  "message": "Go Serverless Webpack (Ecma Script) v1.0! Second module!",
  "input": {
    "resource": "/second",
    "path": "/second",
    "httpMethod": "GET",
...

//}

デプロイしたリソースが不要になった場合は、removeコマンドでまとめて削除できます。

//cmd[リソース削除]{

$ sls remove
Serverless: Getting all objects in S3 bucket...
Serverless: Removing objects in S3 bucket...
Serverless: Removing Stack...
Serverless: Checking Stack removal progress...
...............................
Serverless: Stack removal finished...

// APIもアクセスできなくなっている
$ curl https://xxxxxxx.execute-api.us-east-1.amazonaws.com/dev/second 
curl: (35) SSL peer handshake failed, the server most likely requires a client certificate to connect

//}

==== ローカルで関数を実行する

Serverless Frameworkでは、AWSにデプロイする前に手元の環境で動作を確認することができます。
以下のサンプルのように、AlexaからのリクエストJSONを与えてやることで、Alexaの応答を再現させることも可能です。

//cmd[LaunchRequetをローカル実行]{

$ sls invoke local -f exampleAlexaFunction \
>  -d '{"version":"1.0","session":{"new":true,"sessionId":"amzn1.echo-api.session.123456789012","application":{"applicationId":"amzn1.ask.skill.987654321"},"user":{"userId":"amzn1.ask.account.testUser"},"attributes":{}},"context":{"AudioPlayer":{"playerActivity":"IDLE"},"System":{"application":{"applicationId":"amzn1.ask.skill.987654321"},"user":{"userId":"amzn1.ask.account.testUser"},"device":{"supportedInterfaces":{"AudioPlayer":{}}}}},"request":{"type":"LaunchRequest","requestId":"amzn1.echo-api.request.1234","timestamp":"2016-10-27T18:21:44Z","locale":"ja-JP"}}'

{
    "version": "1.0",
    "response": {
        "reprompt": {
            "outputSpeech": {
                "type": "SSML",
                "ssml": "<speak><p>次は何をしますか？</p></speak>"
            }
        },
        "shouldEndSession": false,
        "outputSpeech": {
            "type": "SSML",
            "ssml": "<speak><p>こんにちは。サンプルスキルへようこそ！次は何をしますか？</p></speak>"
        }
    },
    "userAgent": "ask-node/2.7.0 Node/v10.16.0",

}

//}

Alexaからのリクエストをローカルで再現することはとても煩雑です。
またDevice AddressやISPなどのAlexaが提供するAPIを利用する場合、リクエストのJSONに含まれるAPI tokenの期限が切れるとエラーになるという点にも注意が必要です。
ですが、APLのタッチイベントやスキルイベントなどのシミュレーションしにくいユースケースでちょっと試すなど、知っておくと損はしません。

==== logsコマンドでログを監視する

AWS上にデプロイされたLambda関数のログをServerless Frameworkのコマンドで確認することができます。
このため、AWSのマネジメントコンソールでCloudWatch Logsのページを開かずともLambda関数のデバッグが行なえます。

//cmd[関数の名前を確認後、ログを確認する]{

$ sls info
Serverless: Stack update finished...
Service Information
service: my-first-lambda
stage: dev
region: us-east-1
stack: my-first-lambda-dev
resources: 16
api keys:
  None
endpoints:
  GET - https://xxxxxxx.execute-api.us-east-1.amazonaws.com/dev/second
functions:
  first: my-first-lambda-dev-first
  second: my-first-lambda-dev-second
layers:
  None

$ sls logs -f second
START RequestId: 38234155-d9de-42a4-bd2a-b99f1d6463a4 Version: $LATEST
2020-01-02 16:33:08.195 (+09:00)        38234155-d9de-42a4-bd2a-b99f1d6463a4    INFO    second api has been called!
END RequestId: 38234155-d9de-42a4-bd2a-b99f1d6463a4
REPORT RequestId: 38234155-d9de-42a4-bd2a-b99f1d6463a4  Duration: 3.41 ms       Billed Duration: 100 ms Memory Size: 1024 MB    Max Memory Used: 72 MB  Init Duration: 133.99 ms

START RequestId: e80711b3-0afc-4d55-8c7d-5d0135236264 Version: $LATEST
2020-01-02 16:33:55.574 (+09:00)        e80711b3-0afc-4d55-8c7d-5d0135236264    INFO    second api has been called!2020-01-02 16:33:55.574 (+09:00)     e80711b3-0afc-4d55-8c7d-5d0135236264    ERROR  Invoke Error     {"errorType":"Error","errorMessage":"Something happend!","stack":["Error: Something happend!","    at Runtime.hello [as handler] (webpack:///./second.js?:6:9)","    at Runtime.handleOnce (/var/runtime/Runtime.js:66:25)"]}
END RequestId: e80711b3-0afc-4d55-8c7d-5d0135236264
REPORT RequestId: e80711b3-0afc-4d55-8c7d-5d0135236264  Duration: 4.34 ms       Billed Duration: 100 ms Memory Size: 1024 MB    Max Memory Used: 72 MB  Init Duration: 140.04 ms

//}

なにかエラーが起きたことが確認できますね。
実際にデバッグを行なう場合は、「-t」オプションを付けることでログをストリーミングすることができます。

//cmd[ログのストリーミング]{

$ sls logs -f second -t
START RequestId: 38234155-d9de-42a4-bd2a-b99f1d6463a4 Version: $LATEST
2020-01-02 16:33:08.195 (+09:00)        38234155-d9de-42a4-bd2a-b99f1d6463a4    INFO    second api has been called!
END RequestId: 38234155-d9de-42a4-bd2a-b99f1d6463a4
REPORT RequestId: 38234155-d9de-42a4-bd2a-b99f1d6463a4  Duration: 3.41 ms       Billed Duration: 100 ms Memory Size: 1024 MB    Max Memory Used: 72 MB  Init Duration: 133.99 ms

START RequestId: e80711b3-0afc-4d55-8c7d-5d0135236264 Version: $LATEST
2020-01-02 16:33:55.574 (+09:00)        e80711b3-0afc-4d55-8c7d-5d0135236264    INFO    second api has been called!2020-01-02 16:33:55.574 (+09:00) 

//}

この例では、最後の行がSTARTで終わっています。「-t」オプションを付けない場合、この続きについてはまたコマンドを打つ必要があります。
しかし「-t」オプションをつけていれば、CloudWatch Logsにデータが書き込まれたことが確認出来次第続きが表示されます。

なおこのコマンドは、ログが全く残っていないLambda関数で実行するとエラーになりますのでご注意ください。

//cmd[ログがないのでエラーになるケース]{

$ sls logs -f first
 
  Serverless Error ---------------------------------------
 
  No existing streams for the function
 
  Get Support --------------------------------------------
     Docs:          docs.serverless.com
     Bugs:          github.com/serverless/serverless/issues
     Issues:        forum.serverless.com

//}


==== Lambdaの実行ロールを定義する

AWS IAMの節で紹介した実行ロールの設定についても、Serverless FrameworkのYAMLで定義できます。
Serverless Frameworkでは、providerブロックにIAMのポリシーを定義する形となります。
そのため、YAMLで定義したLambda関数すべてにそのポリシーが適用されることに注意してください。

//listnum[Amazon DynamoDB / Amazon S3へのアクセスを許可した例]{

service:
  name: my-first-lambda
# app and org for use with dashboard.serverless.com
#app: your-app-name
#org: your-org-name

# Add the serverless-webpack plugin
plugins:
  - serverless-webpack

provider:
  name: aws
  runtime: nodejs12.x
  iamRoleStatements:
    - Effect: "Allow"
      Action:
        - "s3:PutObject"
        - "s3:GetObject"
        - "s3:DeleteObject"
      Resource: 
        - "arn:aws:s3:::YOUR_SKILL1_BUCKET/*"
        - "arn:aws:s3:::YOUR_SKILL2_BUCKET/*"
    - Effect: "Allow"
      Action:
        - "dynamodb:CreateTable"
        - "dynamodb:PutItem"
        - "dynamodb:DeleteItem"
        - "dynamodb:GetItem"
      Resource:
        - "arn:aws:dynamodb:AWS_REGION:*:table/YOUR_SKILL1_DB"
        - "arn:aws:dynamodb:AWS_REGION:*:table/YOUR_SKILL2_DB"

functions:
  first:
    handler: first.hello
    events:
      - alexaSkill: amzn1.ask.skill.xxx-xxx-xxx-xxx # スキルのIDがわかっている場合
      - alexaSkill # スキルのIDが不明な場合（デプロイ時にwarningがでます）

//}

==== APIキーなどの情報を安全にデプロイする

外部のサービスと連携させたい場合、API tokenやSecret Keyなどの秘密情報を利用する必要が出てくることがあります。
ソースコードに秘密情報をベタ打ちするのが簡単ではありますが、漏洩のリスクがとても高くおすすめできません。

AWSを利用する場合、AWS Systems ManagerやAWS Secret Managerを利用して秘密情報を管理する方法をおすすめします。

===== Serverless Framework + AWS Systems Manager

AWS Systems Managerにはパラメータの管理を集約して行うことができる「パラメータストア」というサービスが存在します。

Serverless Frameworkを利用してる場合、このパラメータストアに保存した情報をそのままLambda関数の環境変数へ保存することができます。

//cmd[AWS CLIでAWS Systems Managerのパラメータストアにデータを保存する]{

// 平文で保存する
$ aws ssm put-parameter --type String --name "alexa-data" --value "hello"
{
    "Tier": "Standard", 
    "Version": 1
}

// 暗号化して保存する
$ aws ssm put-parameter --type SecureString --name "alexa-secure-data" --value "hello"
{
    "Tier": "Standard", 
    "Version": 1
}

//}

暗号化して保存することで、パラメータストアを直接参照しても内容を確認できないようにすることができます。

//cmd[保存した内容を取得する]{

$ aws ssm get-parameters --names alexa-data  alexa-secure-data
{
    "InvalidParameters": [], 
    "Parameters": [
        {
            "Name": "alexa-data", 
            "LastModifiedDate": 1577952862.687, 
            "Value": "hello", 
            "Version": 1, 
            "Type": "String", 
            "ARN": "arn:aws:ssm:us-east-1:9999999999:parameter/alexa-data"
        }, 
        {
            "Name": "alexa-secure-data", 
            "LastModifiedDate": 1577953029.538, 
            "Value": "AQICAHhfNMi6eMYjbOM5k2AyVWC/SzaOXncGo4MNyn8SK6r6hQE0gsxblrcU+bBbBx25eTpjAAAAAAAAAAAAAAAEHATAeBglghkgBZQMEAS4wEQQMYT3xKvBh5F3LR+lwAgEQgCACY4qdZDYrbL6xE0151WYV4DKyNaqRLNHozb+t/wmk3Q==", 
            "Version": 1, 
            "Type": "SecureString", 
            "ARN": "arn:aws:ssm:us-east-1:9999999999:parameter/alexa-secure-data"
        }
    ]
}

//}

これらのデータは、serverless.ymlを以下のように定義することでLambda関数の環境変数に保存できます。

//listnum[SSMの値をLambdaに保存する]{
provider:
  name: aws
  runtime: nodejs12.x
  environment:
    ALEXA_DATA: ${ssm:alexa-data} # すべてのLambdaの環境変数に保存される

functions:
  first:
    handler: first.hello
    events:
      - alexaSkill
  second:
    handler: second.hello
    environment:
      ALEXA_SECRET_DATA: ${ssm:alexa-secure-data~true} # secondの環境変数にのみ保存される
    events:
      - alexaSkill
      - http:
          method: get
          path: second

//}

secondの関数でそれぞれの環境変数を取得するように実装して、APIとして呼び出してみましょう。

//listnum[環境変数の値を取る実装]{

// eslint-disable-next-line import/prefer-default-export
export const hello = async (event) => {
  const { ALEXA_SECRET_DATA, ALEXA_DATA } = process.env
  return {
    statusCode: 200,
    body: JSON.stringify(
      {
        message: 'Go Serverless Webpack (Ecma Script) v1.0! Second module!',
        ALEXA_SECRET_DATA,
        ALEXA_DATA
      },
      null,
      2
    ),
  };
};

//}

//cmd[呼び出した結果]{

$ curl https://xxxxx.execute-api.us-east-1.amazonaws.com/dev/second

{
  "message": "Go Serverless Webpack (Ecma Script) v1.0! Second module!",
  "ALEXA_SECRET_DATA": "hello",
  "ALEXA_DATA": "hello"
}

//}


===== Serverless Framework + AWS Secret Manager

AWS Systems Managerのパラメータストアだけでも秘密情報を扱えそうですが、AWSには更にセキュアにデータを扱うことができるAWS Secreat Managerというサービスも存在します。
基本的にRDBのパスワードやAPIキー・OAuthのトークンなどはこちらを利用することが推奨されていますので、こちらも触ってみましょう。

こちらもAWS CLIからデータを投入できます。

//cmd[データの保存]{

$ aws secretsmanager create-secret --name SOME_API_TOKEN --description "どこかのサービスのAPIトークンです" --secret-string tok_xxxxxx
{
    "VersionId": "ad575a85-9cb5-4953-8610-2e49efa999999c", 
    "Name": "SOME_API_TOKEN", 
    "ARN": "arn:aws:secretsmanager:us-east-1:999999999:secret:SOME_API_TOKEN-0ZRVY2"
}

// アクセス権限のあるユーザーであればCLIから確認できる
$  aws secretsmanager get-secret-value --secret-id SOME_API_TOKEN
{
    "Name": "SOME_API_TOKEN", 
    "VersionId": "ad575a85-9cb5-4953-8610-2e49efa999999c", 
    "SecretString": "tok_xxxxxx", 
    "VersionStages": [
        "AWSCURRENT"
    ], 
    "CreatedDate": 1577954269.354, 
    "ARN": "arn:aws:secretsmanager:us-east-1:999999999:secret:SOME_API_TOKEN-0ZRVY2"
}

// 不要になった場合は削除する
$  aws secretsmanager delete-secret --secret-id SOME_API_TOKEN

//}

AWS SecretManagerを利用する場合は、Lambda関数の環境変数ではなく都度復号化する形となります。
そのためserverless.ymlでAWS Secret Managerへのアクセスを許可しましょう。

//listnum[AWS Secret Managerへのアクセスを許可する]{

provider:
  name: aws
  runtime: nodejs12.x
  iamRoleStatements:
    - Effect: "Allow"
      Action:
        - "secretsmanager:GetSecretValue"
      Resource: 
        - "arn:aws:secretsmanager:us-east-1:*:secret:SOME_API_TOKEN*"

}

続いてLambda関数側で復号化処理を行います。

//listnum[Node.jsでの復号化処理]{

// eslint-disable-next-line import/prefer-default-export
import { SecretsManager } from 'aws-sdk'

export const hello = async (event) => {
  const client = new SecretsManager()
  const { SecretString:token } = await client.getSecretValue({
    SecretId: 'SOME_API_TOKEN'
  }).promise()
  return {
    statusCode: 200,
    body: JSON.stringify(
      {
        message: 'Go Serverless Webpack (Ecma Script) v1.0! Second module!',
        token
      },
      null,
      2
    ),
  };
};

//}

これをデプロイすると、ソースコードにもLambda関数の環境変数にも記述していないAPIトークンの値が無事取得できていることがわかります。

//cmd{

$ curl https://xxxxxx.execute-api.us-east-1.amazonaws.com/dev/second
{
  "message": "Go Serverless Webpack (Ecma Script) v1.0! Second module!",
  "token": "tok_xxxxxx"
}

//}

このようにAWS Systems ManagerやAWS Secret Managerを用いることで、秘密情報を安全に保持し、利用することができます。
AWS Secret Managerのほうが安全に運用するための機能が豊富ですが、AWS Systems Managerのパラメータストアは無料で利用できるというメリットがあります。

両方の使い方を理解して、安全かつ低コストな運用を目指しましょう。

==== やっておくと便利な設定いろいろ

この他にも細かい設定を行うことで、より効率的にAWSリソースを管理することができます。

ここからは筆者が行っているTipsについて一部紹介します。

===== ログの保存期間を短くする

Serverless FrameworkでCloudWatch Logsのログ保存期間を変更できます。
CloudWatch Logsのログは5GBを超えると請求が発生します。
そのため有効期限を設定することで保存するログの量を減らしてコストを節約することが期待できます。

//listnum[Amazon DynamoDB / Amazon S3へのアクセスを許可した例]{

service:
  name: my-first-lambda
# app and org for use with dashboard.serverless.com
#app: your-app-name
#org: your-org-name

# Add the serverless-webpack plugin
plugins:
  - serverless-webpack

provider:
  name: aws
  runtime: nodejs12.x
  logRetentionInDays: 14 # 14日を有効期限に設定
...

//}

===== デプロイ時のS3バケットを固定する

Serverless Frameworkはデプロイの際、YAMLファイルをCloudFormationテンプレートに変換します。
この変換したファイルとデプロイするソースコードのzipファイルをアップロードするために、プロジェクト毎にAmazon S3にバケットが作成されます。

しかしこのS3バケットには1アカウント最大100までという制限があります。
そこでデプロイするS3バケットをYAML側で指定することで、新しいプロジェクトをデプロイする度にS3バケットが作成されることを防ぐことができます。

//listnum[デプロイバケットを指定する]{


service:
  name: my-first-lambda
# app and org for use with dashboard.serverless.com
#app: your-app-name
#org: your-org-name

# Add the serverless-webpack plugin
plugins:
  - serverless-webpack

provider:
  name: aws
  runtime: nodejs12.x
  logRetentionInDays: 14
  deploymentBucket: YOUR_S3_BUCKET_NAME

//}


===== 古いバージョンのソースコードを自動削除する

デプロイされたLambda関数のソースコードは、過去のバージョンとして残り続けます。
ロールバックするには便利ですが、コードストレージの残容量を圧迫しますので、不要な分は削除するようにしています。

Serverless Frameworkの場合は、便利なプラグインがサードパーティから提供されていますので、こちらを用います。

//cmd[install]{

$ sls plugin install -n serverless-prune-plugin

//}

//listnum[pruneプラグインを適用する]{

plugins:
  - serverless-webpack
  - serverless-prune-plugin

custom:
  prune:
    automatic: true
    number: 3

provider:
  name: aws
  runtime: nodejs12.x
//}

このようにすることで、デプロイする度に直近3世代より古いコードについては自動的に削除してくれるようになります。

//note[Note: Serverless FrameworkだけでAlexaスキル開発を完結する]{

Serverless Frameworkでは、Alexaの対話モデルなども管理することができます。

//cmd[Alexaプロジェクトのセットアップ]{

$ sls create -t aws-alexa-typescript -p alexa

//}

こちらは「serverless-alexa-skills」というプラグインを利用して、Serverless FrameworkのYAMLとコマンドだけでAlexaスキルを作成・デプロイできます。

serverless-alexa-skills
https://www.npmjs.com/package/serverless-alexa-skills

//listnum[serverless.yml]{

service:
  name: alexa
# app and org for use with dashboard.serverless.com
#app: your-app-name
#org: your-org-name

plugins:
  - serverless-webpack
  - serverless-alexa-skills

provider:
  name: aws
  runtime: nodejs12.x

custom:
  alexa:
    # Step 1: Run `sls alexa auth` to authenticate
    # Step 2: Run `sls alexa create --name "Serverless Alexa Typescript" --locale en-GB --type custom` to create a new skill
    skills:
      # Step 3: Paste the skill id returned by the create command here:
      - id: amzn1.ask.skill.xxxx-xxxx-xxxx-xxxx-xxxx
        manifest:
          publishingInformation:
            locales:
              en-GB:
                name: Serverless Alexa Typescript
          apis:
            custom:
              endpoint:
                # Step 4: Do your first deploy of your Serverless stack
                # Step 5: Paste the ARN of your lambda here:
                uri: arn:aws:lambda:[region]:[account-id]:function:[function-name]
                # Step 6: Run `sls alexa update` to deploy the skill manifest
                # Step 7: Run `sls alexa build` to build the skill interaction model
                # Step 8: Enable the skill in the Alexa app to start testing.
          manifestVersion: '1.0'
        models:
          en-GB:
            interactionModel:
              languageModel:
                invocationName: serverless typescript
                intents:
                  - name: HelloIntent
                    samples:
                      - 'hello'

functions:
  alexa:
    handler: handler.alexa
    events:
      - alexaSkill: ${self:custom.alexa.skills.0.id}


//}

YAMLのコメントで手順が説明されていますが、Alexaの開発者アカウントとの連携などの作業が手動で必要となります。
このプラグインを作成された方（日本人）が、自身のブログで使い方などを紹介されていますので、ぜひこちらもお試しください。

Alexa Skillの開発をServerless Frameworkだけで完結するための「Serverless Alexa Skills Plugin」の紹介
http://marcy.hatenablog.com/entry/2017/12/14/000047

//}


==== Serverless Frameworkを選ぶのが正解とは限らない

本章ではInfrastructure as Codeの例として、筆者がよく利用しているServerless Frameworkを紹介しました。
ですが、この他にもAWS CloudFormationやAWS SAM、AWS CDKといったAWSが公式で提供しているツール・ライブラリやterraformなどのOSSも存在します。

それぞれのツールにはそれぞれのメリット・デメリットがあります。
Serverless FrameworkはAWS Lambdaを軸にリソースを定義する仕様です。
そのためそれ以外のサービスを中心にする場合には気がつけばCloudFormationを書いていたというケースが少なくありません。

AWS CDKをTypeScriptで用いることでIDEの入力補助を使えることがメリットになる場合もありますし、
手動で作成したリソースが多い場合はterraformで管理するほうが作り直しの手間がなくてよいかもしれません。

これをきっかけに、さまざまな構成管理ツールについて触れていただくことができればと思います。

==== Infrastructure as Codeまとめ

 * コードで管理することで、複雑な構成になっても管理しやすい
 * Serverless FrameworkやAWS CloudFormationなど、ツールは多種多様
 * AWS Systems Manager / AWS Secrets Managerを使うことで秘密情報も安全に扱える

//note[Note: DB / メディアストレージとそれ以外は個別管理しよう]{

コードでの管理はとても便利です。簡単に複雑な構成を立ち上げて、すぐに削除することができます。
ですがその便利さ故に大事故が発生するケースも存在します。

本章のサンプルでは、Amazon DynamoDBやAmazon S3のようなDB、ストレージを含めませんでした。
これは、Lambda関数やAPIのような「リソースごと削除して作り直すことがあるリソース」と「気軽に削除してはいけないリソース」を分けて管理するためです。

Lambda関数についてはソースの内容や名前が変わっていなければ一度削除して作り直しても特に問題はありません。
APIについても独自ドメインを割り当て直せばすぐに復旧できます。
ですがデータベースの中身やアップロードされたメディアファイルなどは、復旧が難しい場合や時間がかかるケースが少なくありません。

万が一の事故を防ぐためにも、DBやストレージについては個別で管理することをおすすめします。
コードで管理する場合には、プロジェクトを分けて誤爆などが起きないようにするとよいでしょう。

//}


== もっとAWSを使いたおそう

ここまで、AWS上でAlexaスキルを開発する場合の基本的な使い方について紹介してきました。
最後はもう少し踏み込んだ使い方のアイディアについて紹介します。

=== Alexaスキルを公開して、100ドルクーポンを獲得しよう！
AWSでは、Alexaスキルを公開している開発者向けに毎月100ドルの割引を受けることができるクーポンを配布しています。(2019年12月現在)
https://developer.amazon.com/ja-JP/alexa/alexa-skills-kit/alexa-aws-credits

このクーポンは、Alexaスキルを公開している必要があり、初めてスキルを作るという方には少しハードルが高いかもしれません。
しかし割引の対象にAmazon EC2やAmazon CloudFrontのようなAlexa以外でも利用する機会のあるサービスまで対象と、とてもお得な内容です。

また、利用金額の合計が100ドルを超えた場合、1度限りですが追加で100ドルのクーポンを自動付与するという仕組みも用意されています。
つまり、AlexaスキルをAWS上で利用する場合、月額200ドルまでであれば無料で利用することが可能ということです。
普段であればなかなか利用する機会のないサービスなども、このクーポンを取得してトライしてみましょう！

=== Mobile / web applicationと連携

AWSにはモバイルアプリケーションなどのユーザー管理に利用できるCognito User Poolsというサービスがあります。
このサービスを利用することで、自分のweb applicationにログイン機能を追加することができます。

Cognito User Poolsの特徴の１つとして、OAuth2での連携ができるという点があります。
Alexaスキルでは、外部のサービスのユーザー情報とAlexaのユーザー情報を紐付けるために「アカウントリンク」という仕組みが存在します。
この仕組みを利用して連携する場合には、OAuth2を用いる必要がありますが、Cognito User Poolsを利用している場合には、この連携がとても簡単に行うことができます。

アカウントリンクの方法については、クラスメソッドという会社のブログにて紹介されています。
興味があるかたは、こちらもぜひ挑戦してみてください。

[日本語Alexa] Cognito User Pool によるアカウントリンク@<br>{}
https://dev.classmethod.jp/cloud/alexa-account-link-cognito/

=== 機械学習APIを活用しよう

Alexaでの会話について考えていると、「過去の使われ方をもとに、パーソナライズした体験を提供したい」と思うことがあります。
そんなときに便利なのが、AWSの機械学習系APIサービスです。

2019年12月時点では、AWSでは以下のような機械学習APIサービスが提供されています。

 * Amazon Comprehend テキスト内でインサイトや関係性を検出
 * Amazon Forecast CSVなどのデータをもとに時系列の予測を実行
 * Amazon Polly  テキストを音声に変換する 
 * Amazon Rekognition  動画や音声の分析
 * Amazon Textract ドキュメントからテキストやデータを抽出
 * Amazon Translate  テキストの翻訳
 * Amazon Transcribe 音声のテキスト変換
 * Amazon Personalize  リアルタイムのパーソナライズとレコメンド 
 * Amazon Kendra [プレビュー]機械学習を原動力とする高精度のエンタープライズ検索サービス

この他にもTernsorflowなどの機械学習系OSSを実行させるためのサービスなども用意されていますので、パーソナライズにチャレンジしてみたい方はぜひ挑戦してみてください。

== 「もっと！」という方に、AWSコミュニティの紹介

「AWSをもっと使いたおしたい」という方には、AWSのユーザーグループやAWS公式イベントへの参加をおすすめしています。

=== Japan AWS User Group (JAWS-UG)
AlexaのユーザーグループとしてAAJUGがあるように、AWSのユーザーグループとして「JAWS-UG」というコミュニティが存在します。
このコミュニティは日本各地でイベントを開催しており、モバイルアプリケーションからIoT・機械学習・HPCと幅広いテーマを取り扱っています。
また、毎年「JAWS Days（東京）」と「JAWS Festa（各地域持ち回り）」という2つのコミュニティカンファレンスも開催されており、個人開発から大企業での事例まで幅広く話を聞くことができます。

コミュニティサイト（https://jaws-ug.jp/）から開催予定のイベント情報を確認できます。AWSについてもっと知りたいという方は、ぜひこちらも参加してみてください。

=== AWS公式イベント
また、AWSが公式に開催するイベントとして、AWS Summitというイベントも存在します。
こちらは東京と大阪で開催されており、入門向けコンテンツなども用意されています。

さらに海外にはなりますが、毎年数多くの新サービス発表が行われる巨大カンファレンス「AWS re:invent」というイベントもラスベガスで開催されています。
こちらではAlexaスキル開発をAWSで行うセッションやワークショップなども毎年開催されていますので、アメリカのAlexa開発事情を知ることもできるイベントです。

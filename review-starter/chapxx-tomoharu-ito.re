
= AlexaスキルをWebサービスとしてローンチしよう

//image[tag][]{
//}

//flushright{
著者：haruharuharuby（@<href>{https://twitter.com/haruharuharuby/,@haruharuharuby}）
//}


== はじめに


こんにちは。@haruharuharuby です。オランダ在住のフリーランスプログラマーです。Alexaのユーザーコミュニティ AAJUG (Amazon Alexa Japan User Group) の管理人です。 Alexa Champion として、AlexaとVUIの普及をコミュニティから広げる活動をしています。@<br>{}
本章では、Alexaスキル(以下、スキル) を Webサービスとして構築する方法についてお話します。今日では多くの開発者が、スキルのエンドポイントとして AWS Lambda を利用していると思います。（今となっては、スキルのエンドポイントにWebサービスを選択することは、世の中にほとんどないのではないか？とさえ思っています）@<br>{}
スキルのエンドポイントをWebサービスとして構築するには多少の手間が必要です。しかし、できてしまえば AWS Lambda で構築するよりも高いポータビリティを得られる場合があります。また、自社のドメインの一貫としてスキルを提供することもできるようになります。@<br>{}
本章では、Webサービスとしてスキルのエンドポイントを構築する手法を学び、そのメリット、デメリットについて考察していきたいと思います。今後のスキル開発において新たなエッセンスになってもらえれば幸いです。


===== 本章の対象読者@<br>{}

本章の対象読者は、次のような方です。

 * 自社のWebサービスとしてスキルを作ってみたい方
 * スキルコードを Dockerize して、ポータビリティを向上させたいと思っている方
 * スキルの中に重たい処理があり、スキルを実行するインフラにパフォーマンスが必要な方
 * AWS Lambda の コールドスタート を完璧に回避したい方
 * Node.js、Python以外でスキルを作ってみたい方

===== 本章の前提知識@<br>{}

本章は、ステップバイステップ で Webサービスを構築できるように配慮した構成になっていますが次の知識を前提としています。

 * Node.js
 * Typescript
 * Express.js(Node.js 製の Webアプリケーションフレームワーク)
 * AWS CLI
 * AWS Cloud Development Kit (以下、AWS CDK)
 * Alexa Skills Kit for Node.js (以下、ASK SDK)

また、本章では次のAWSサービスとDockerを使います。これらのサービスに関しても使用経験のあることが望ましいです。

 * AWS Fargate
 * Docker
 * Amazon EC2
 * Amazon Elastic Container Service (以下、Amazon ECS)
 * Amazon Elastic Container Registry (以下、Amazon ECR)
 * AWS Certificate Manager (以下、AWS ACM)　


//quote{
本章で使うライブラリのインストール方法については説明を割愛しています。説明の都合上 パッケージマネージャーは、npm と yarn の両方を使います。これらのインストール方法は、それぞれのライブラリのWebサイトを参照してください。
//}

===== 本章の内容@<br>{}

本章では、AWS Fargate を使って AWS 上に Webサーバーを構築します。Webサーバーは Express.js を利用します。AWS Fargate は、AWSが提供するマネージドコンテナサービスです。コンテナに必要なメモリとCPUパワーを設定し、それに合わせたインフラストラクチャを自動的に構成します。Express.js は、javascript で記述された 軽量 Webアプリケーションフレームワークです。Node.js 上で動作し軽量のWebサーバーを手軽に構築することができます。
　@<br>{}

====== AWS Fargate@<br>{}
@<href>{https://aws.amazon.com/jp/fargate/}
　@<br>{}

====== Express.js@<br>{}
@<href>{https://expressjs.com/}
　@<br>{}

===== 本章の構成@<br>{}

本章では、まず最初にWebサービスとしてのAlexaの仕様を確認し、次に Docker、AWS Fargate、Express.js を使ってWebサービスを構築します。


== AlexaスキルをWebサービス化する要件


AlexaスキルをWebサービスとしてホストするには、次に示す５つの要件を満たす必要があります。

 1. インターネットでアクセス可能であること
 1. ポート443上のHTTPリクエストを受け入れること
 1. Amazon信頼済み証明書を使用したSSL/TLS上のHTTPをサポートしていること。ウェブサービスのドメイン名が、証明書のSubject Alternative Names（SAN）セクションにあること
 1. 受信するリクエストはAlexaからのものであることを検証できること
 1. Alexa Skills Kitインターフェースに準拠していること

公式 ドキュメントは以下にあります。@<br>{}

====== 「カスタムスキルをウェブサービスとしてホスティングする」@<br>{}
@<href>{https://developer.amazon.com/ja-JP/docs/alexa/custom-skills/host-a-custom-skill-as-a-web-service.html}
　@<br>{}

1から3は、サービスをホストするWebサーバーの要件です。一度構築してしまえば頻繁に作り直すということはないでしょう。4と5は スキルのコードに実装します。今回は、インフラストラクチャの構築とデプロイメントに AWS CDK を使います。AWS CDK を使うと、これまで YAMLやJSONで記述していたインフラの構成をプログラマブルに記述できます。また、豊富なクラスのセットが提供されているため、コード量を減らすことが可能です。4と5については、この章で紹介するコードを参考に実装すれば大きな手間なく実装できるでしょう。今回紹介するスキルのコードは、ASK SDK も利用しますので、スキル開発の経験があれば抵抗感は少ないはずです。@<br>{}
　@<br>{}
Alexaのリクエストとレスポンスは @<strong>{Alexa Skill Kitインターフェース} に準拠する必要があります。ASK SDK の登場以降にスキル開発を始めたかたの中には、実際にAlexaデバイスとスキルの間でやりとりされている HTTPメッセージ を意識していないかたも多いのではないでしょうか。この章に目が止まったならちょうど良いタイミングです。Alexaデバイスとスキルの間でやりとりされている「生」のメッセージを理解しましょう！@<br>{}
　@<br>{}
スキルが受け取るHTTPメッセージを次に示します。

//emlist[リクエストヘッダ][HTTP]{
POST / HTTP/1.1
Content-Type : application/json;charset=UTF-8
Host : your.application.endpoint
Content-Length :
Accept : application/json
Accept-Charset : utf-8
Signature: xxxxx
SignatureCertChainUrl: https://s3.amazonaws.com/echo.api/echo-api-cert.pem
//}


HTTPヘッダーには、Signature、SignatureCertChainUrl という2つのヘッダがAlexa側から送信されます。これは@<strong>{リクエストがAlexaデバイスから送られたこと}を保証するための情報です。Signatureは、リクエストに対するハッシュ値であり、SignatureCertChainUrlはAlexaがWebサービスにリクエストを送る際の暗号化に使われる証明書（公開鍵）です。



//emlist[リクエストボディ][json]{
{
  "version": "1.0",
  "session": {
    "new": true,
    "sessionId": "amzn1.echo-api.session.[unique-value-here]",
    "application": {
      "applicationId": "amzn1.ask.skill.[unique-value-here]"
    },
    "attributes": {
      "key": "string value"
    },
    "user": {
      "userId": "amzn1.ask.account.[unique-value-here]",
      "accessToken": "Atza|AAAAAAAA...",
      "permissions": {
        "consentToken": "ZZZZZZZ..."
      }
    }
  },
  "context": {
    "System": {
      "device": {
        "deviceId": "string",
        "supportedInterfaces": {
          "AudioPlayer": {}
        }
      },
      "application": {
        "applicationId": "amzn1.ask.skill.[unique-value-here]"
      },
      "user": {
        "userId": "amzn1.ask.account.[unique-value-here]",
        "accessToken": "Atza|AAAAAAAA...",
        "permissions": {
          "consentToken": "ZZZZZZZ..."
        }
      },
      "person": {
        "personId": "amzn1.ask.account.[unique-value-here]",
        "accessToken": "Atza|BBBBBBB..."
      },
      "apiEndpoint": "https://api.amazonalexa.com",
      "apiAccessToken": "AxThk..."
    },
    "AudioPlayer": {
      "playerActivity": "PLAYING",
      "token": "audioplayer-token",
      "offsetInMilliseconds": 0
    }
  },
  "request": {}
}
//}


リクエストボディには、version, session, context というトップレベルオブジェクトが格納されます。これは、ASK SDK でいうところの handlerInput.requestEnvelope にアクセスして取得できる内容です。ASK SDK をつかう場合、session オブジェクトは sessionAttributeManager オブジェクトで読み書きするので、これらのメッセージの構成は意識しなくてもよいようになっています。

次に スキルからデバイスに送るレスポンスを見てみましょう。

//emlist[レスポンスヘッダ][HTTP]{
HTTP/1.1 200 OK
Content-Type: application/json;charset=UTF-8
Content-Length:
//}


レスポンスヘッダには、特徴的なところはありません。ごく一般的な HTTP レスポンスです。

//emlist[レスポンスボディ][JSON]{
{
  "version": "string",
  "sessionAttributes": {
    "key": "value"
  },
  "response": {
    "outputSpeech": {
      "type": "PlainText",
      "text": "Plain text string to speak",
      "playBehavior": "REPLACE_ENQUEUED"      
    },
    "card": {
      "type": "Standard",
      "title": "Title of the card",
      "text": "Text content for a standard card",
      "image": {
        "smallImageUrl": "https://url-to-small-card-image...",
        "largeImageUrl": "https://url-to-large-card-image..."
      }
    },
    "reprompt": {
      "outputSpeech": {
        "type": "PlainText",
        "text": "Plain text string to speak",
        "playBehavior": "REPLACE_ENQUEUED"             
      }
    },
    "directives": [
      {
        "type": "InterfaceName.Directive"
        (...properties depend on the directive type)
      }
    ],
    "shouldEndSession": true
  }
}
//}


レスポンスボディには、Alexaのレスポンスでおなじみのオブジェクト群が並びます。ASK SDKでいうところの handlerInput.requestEnvelope.responseBuilder で操作するターゲットが格納されていることがわかります。トップレベルの構成要素は、「version」「sessionAttributes」「response」であり、セカンドレベルオブジェクトとしては、「response」の下に「outputSpeech」「card」「reprompt」「directive」の４つです。この構成は、Alexaが2014年にアメリカでローンチされてから今も変わっていません。昨年は、Reminder、Proactive API、InSkillPurchasing などのさまざまな付加機能が発表され、日本でも利用可能になりました。これらの付加機能はすべて「directive」メッセージとして送信していることが想像できると思います。


//quote{
Alexaの世界では、デバイスからスキルへのメッセージを「イベント」、スキルからデバイスへのメッセージを「ディレクティブ」と呼びます。
//}

カスタムスキルのJSONレスポンスの詳細は、developer.amazon.com の次のドキュメントを参照してください。@<br>{}

====== 「カスタムスキルのJSONインターフェースのリファレンス」@<br>{}
@<href>{https://developer.amazon.com/ja-JP/docs/alexa/custom-skills/request-and-response-json-reference.html}
　@<br>{}

Webサービスとしてのスキルの仕様がわかったところで、早速エンドポイントを構築していきましょう。



== AWS Fargate による Alexa Webサービスの構築


それでは、AlexaのWebサービス基盤を作っていきましょう。



言語は、Node.js(v12.x) を利用します。別のクラウドベンダーや Node.js 以外の言語で構築することもできます。Node.js を選んだ理由は、開発者の多くが Node.js を利用していることと、ASK SDKが使えるからです。前節の要件を満たせば、どのような環境、言語でもWebサービスを構築できます。


=== package.json


適当なところにフォルダを作り、package.json ファイルを作りましょう。
次のコマンドを実行します。


//cmd[]{
mkdir alexa_web_service
cd alexa_web_service
yarn init
//}


コマンドを実行すると対話形式でいくつか入力を求められます。入力が終わると、package.json が生成されています。
次のように書き換えます。


//emlist[package.json][json]{
{
  "name": "fargate_alexa",
  "version": "0.0.1",
  "description": "alexa express server on docker(fargate)",
  "main": "server.js",
  "author": "haruharuharuby",
  "license": "MIT",
  "scripts": {
    "start": "node dist/server.js",
    "build": "tsc",
    "watch": "tsc -w"
  },
  "dependencies": {
    "@types/express": "^4.17.2",
    "express": "^4.17.1",
    "typescript": "^3.7.5"
  }
}
//}


ファイルを作ったら、ターミナルから次のコマンドを実行してください。
Express.js と TypeScript がインストールされます。


//cmd[]{
yarn install
//}

=== tsconfig.json


次に、TypeScript の設定を行います。
次のコマンドを実行します。


//cmd[]{
tsc --init
//}


tsconfig.json が生成されます。次のように変更しましょう。


//emlist[tsconfig.json][json]{
{
  "compilerOptions": {
    "target": "es2016",
    "module": "commonjs",
    "lib": ["es2016"],
    "outDir": "./dist",
    "strict": true,
    "esModuleInterop": true
  },
  "exclude": [
    "node_modules"
  ]
}
//}


TypeScriptについて詳しく知りたい方は、田中みそ さんの担当された第6章の@<secref>{chapxx-miso-tanaka|typescript-setup}を参照してください。


=== server.js


先ほどと同じフォルダの下に、server.ts というファイル を作りましょう。


//emlist[server.ts][typescript]{

import express from 'express'

const app = express()
const PORT = 80;
const HOST = '0.0.0.0';

// スキルのリクエストを受ける予定のパス https://alexa.hugtech.io を想定
app.post('/', (req: express.Request, res: express.Response) => {
  console.log(req)
  res.send('Hello World');
})

// 後で構築する Application Load Balancer の ヘルスチェックを受けるために HTTP GET をリッスンしておく
app.get('/', (req: express.Request, res: express.Response) => {
  console.log(req)
  res.send('Hello World');
})

app.listen(PORT, HOST);
console.log(`Running on http://${HOST}:${PORT}`);

//}


この時点では、Webサーバーとして起動して、ルートパス(/)にアクセスすると、'Hello World' と返すだけの簡易なものです。
ルートパス以外にアクセスすると 404 (Not Found) が返ります。



次のコマンドを実行して、dist フォルダ に コンパイルされた JavaScript(server.js) が格納されることを確認しましょう。


//cmd[]{
yarn run build
//}

=== Dockerfile


次は、同じフォルダに AWS Fargateで起動させるDockerコンテナの設定を作りましょう。先ほどと同じフォルダに Dockerfile というファイルを作ります。Dockerfileの内容は次のとおりです。


//emlist[Dockerfile][Dockerfile]{
FROM node:12

# コンテナ上のワーキングディレクトリ
WORKDIR /usr/src/app

# 必要なパッケージ(ライブラリ)のリスト
COPY package.json ./

# yarn.lock に記載のあるパッケージをコンテナにインストール
RUN yarn install

# コンパイルされたJavascriptをコンテナにコピー
COPY ./dist .

# Webサーバーとして80ポートを解放
EXPOSE 80

# Express.js を起動
CMD [ "node", "server.js" ]

//}

=== .dockerignore


次の .dockerignore ファイルも作っておきましょう。.dockerignore ファイルには、開発時にローカルマシン上で必要なだけで、実際に運用するときには必要ないファイルを記述します。


//emlist[.dockerignore]{
node_modules
npm-debug.log
//}

=== ローカルマシンで実行して確認


Dockerfile まで作ったので一度ビルドして、Webサーバーがローカルで起動することを確認しましょう。


//cmd[]{
# -t <<Tag名>> オプションを指定して、docker イメージに名前をつけておきます。
docker build -t haruharuharuby/alexa_fargate .
docker images
//}


次のように Dockerイメージが表示されれば成功です。


//cmd[]{
REPOSITORY                      TAG       IMAGE ID      CREATED               SIZE
haruharuharuby/alexa_fargate    latest    xxxxxxxxxx    About a minute ago    915MB
//}


Dockerイメージができたので起動してみましょう。次のコマンドを実行してください。


//cmd[]{
# 先ほど作ったコンテナイメージを起動 ローカルマシンで HTTPポート80番は使用されてるので別のポート(例では48000)で起動し、コンテナの起動したポート80番にリクエストを転送します。
docker run -p 48000:80 -d haruharuharuby/alexa_fargate
3c8e7b11033973d26ff68a4372f8cbd2dd4bba410108868034exxxxxxxxxxx
[13時24分26秒] [~/hugtech/fargate_alexa] 

# 起動してるかを確認 
-> % docker ps
CONTAINER ID        IMAGE                          COMMAND                  CREATED             STATUS              PORTS                     NAMES
3c8e7b110339        haruharuharuby/alexa_fargate   "docker-entrypoint.s…"   4 seconds ago       Up 3 seconds        0.0.0.0:48000->80/tcp   heuristic_ishizaka
[13時24分29秒] [~/hugtech/fargate_alexa]

# 叩いてみましょう
curl -X POST http://localhost:48000   
Hello World%              

curl -X GET http://localhost:48000   
Hello World%    
//}


Webサーバーとしてコンテナが動作するようになりました。


=== ローカルのDockerイメージを Amazon ECR に Push


Webサーバーとして動く Dockerコンテナの準備ができたので、スキルのコードを書く前に AWS Fargate にデプロイしておきます。



先ほど haruharuharuby/alexa_fargate というタグでローカルでビルドしたDockerイメージを Amazon ECR に Push します。PushしたDockerイメージは AWS Fargate から参照されます。
作業には、AWS CLI を使います。



次に示すコマンドを順に実行してください。


//cmd[]{
# Amazon ECR に リポジトリを作る
aws ecr create-repository --repository-name alexa-web-service --region us-east-1 --profile self

# 作成されたリポジトリ
{
    "repository": {
        "repositoryArn": "arn:aws:ecr:us-east-1:YOUR_AWS_ACCOUNT_ID:repository/alexa-web-service",
        "registryId": "YOUR_AWS_ACCOUNT_ID",
        "repositoryName": "alexa-web-service",
        "repositoryUri": "YOUR_AWS_ACCOUNT_ID.dkr.ecr.us-east-1.amazonaws.com/alexa-web-service",
        "createdAt": 1579963239.0,
        "imageTagMutability": "MUTABLE",
        "imageScanningConfiguration": {
            "scanOnPush": false
        }
    }
}

# ローカルイメージを ECR に Push
$(aws ecr get-login --no-include-email --region us-east-1 --profile self)e

WARNING! Using --password via the CLI is insecure. Use --password-stdin.
Login Succeeded

# Amazon ECRのイメージにタグ付け
docker tag haruharuharuby/alexa_fargate:latest YOUR_AWS_ACCOUNT_ID.dkr.ecr.us-east-1.amazonaws.com/alexa-web-service:latest

# Amazon ECRに Push
docker push YOUR_AWS_ACCOUNT_ID.dkr.ecr.us-east-1.amazonaws.com/alexa-web-service:latest

# Pushされたことを確認
aws ecr list-images --repository-name alexa-web-service --region us-east-1 --profile self
{
    "imageIds": [
        {
            "imageDigest": "sha256:xxxxxxxx",
            "imageTag": "latest"
        }
    ]
}
//}


コンテナに 'alexa-web-service' というリポジトリを作成し、コンテナのイメージをPushできました。


=== Fargateにデプロイ


Amazon ECR のリポジトリにイメージがPushできたので、AWS Fargate にデプロイするコードを AWS CDK で記述しましょう。
CDKを動作させるためのフォルダをひとつ作って、そこに移動しましょう。


//cmd[]{
mkdir AlexaFargateCdk
cd AlexaFargateCdk
//}


次に、AWS CDK の初期化と必要なライブラリのインストールです。


//cmd[]{
mkdir AlexaFargateCdk
cdk init --language typescript
npm install @aws-cdk/aws-ec2 @aws-cdk/aws-ecs @aws-cdk/aws-ecs-patterns @aws-cdk/aws-ecr @aws-cdk/aws-certificatemanager @aws-cdk/aws-route53
//}


インストールが終わったら、lib フォルダに xxxxx_cdk-stack.ts という テンプレートファイルが生成されています。
このファイルに次の記述を追加します。


//emlist[xxxxx_cdk-stack.ts][typescript]{

import * as cdk from '@aws-cdk/core';
import ec2 = require("@aws-cdk/aws-ec2");
import ecs = require("@aws-cdk/aws-ecs");
import ecs_patterns = require("@aws-cdk/aws-ecs-patterns");
import ecr = require("@aws-cdk/aws-ecr")
import acm = require("@aws-cdk/aws-certificatemanager")
import route53 = require("@aws-cdk/aws-route53")

export class FargateAlexaCdkStack extends cdk.Stack {
  constructor(scope: cdk.Construct, id: string, props?: cdk.StackProps) {
    super(scope, id, props);

    const name = 'AlexaWebService'

    // リポジトリは先ほど ECR に 作成したリポジトリを参照
    const repo = ecr.Repository.fromRepositoryName(this, `${name}Ecr`, 'alexa-web-service')

    // Dockerコンテナを動作させる VPC(Virtual Private Cloud) ネットワークを作成
    const vpc = new ec2.Vpc(this, `${name}Vpc`, {
      maxAzs: 3
    });

    // 証明書を取得。AWS ACMで事前に作成しておいたものを取得
    const certificate = acm.Certificate.fromCertificateArn(this, `${name}Acm`, 'arn:aws:acm:us-east-1:xxxxxxxx:certificate/xxxxxxxxx')
    const cluster = new ecs.Cluster(this, `${name}Cl`, {
      vpc: vpc
    });

    // サービスを提供するドメインの情報を Route 53 で設定。Route 53上に ゾーンを事前に作成しておく
    const hostedZone = route53.HostedZone.fromHostedZoneAttributes(this, `${name}Hz`, {
      hostedZoneId: 'XXXXXXX',　// Route 53 の Hosted Zone Id
      zoneName: 'hugtech.io' // Hosted Zone が管理するドメイン
    })

    // Fargate サービスを作成、合わせて Application Load Balancer も作られる
    new ecs_patterns.ApplicationLoadBalancedFargateService(this, `${name}Lb`, {
      cluster: cluster,
      certificate: certificate,
      cpu: 512,
      desiredCount: 3,
      taskImageOptions: { image: ecs.ContainerImage.fromEcrRepository(repo) },
      memoryLimitMiB: 2048,
      publicLoadBalancer: true,
      domainName: 'alexa.hugtech.io',
      domainZone: hostedZone,
    });
  }
}

//}


スクリプトの要点は次の４点です。

 * Fargateが利用するイメージに AWS ECR に Push した リポジトリ 'alexa-web-service' を指定する
 * AlexaのWebサービスは HTTPS でホストする必要があるため、事前に AWS Certification Manager で証明書を作成し、ARN(Amazon Resource Name)を取得
 * コンテナは HTTP 80 番でリッスンしているため、前段に Application Load Balancer を設置し、HTTPS(443) -> HTTP(80) へポートフォワードさせる
 * ドメインは Route 53 で事前に作成し、HostedZoneId を取得しておく



テンプレートファイルが変更できたら、デプロイする準備は完了です。次のコマンドでデプロイしてみましょう。


//cmd[]{
npm run build
cdk deploy
//}


上のコマンドを実行すると、AWS CloudFormation 経由で 必要なインフラが作成されます。
curl コマンドで、Webサーバーがデプロイされたか確認してみましょう。
次のような結果になるはずです。


//cmd[]{
curl -X POST https://alexa.hugtech.io
Hello World%

curl -X GET https://alexa.hugtech.io
Hello World%
//}


おつかれさまでした。ここまでで WebサーバーをAWS上にデプロイすることができました。
次はいよいよ、Webサーバーを Alexa Web サーバーとして動作させるためのコードを書いていきます。


=== スキルのコード


まずは、スキルのコードを記述しましょう。AWS Lambda を利用する場合と全く同じで、ASK SDK を使います。
プロジェクトのルートフォルダに移動して、skill.ts というファイル名で保存しましょう。


//cmd[]{
cd ..
//}

//emlist[skill.ts][typescript]{
import * as Alexa from 'ask-sdk-core'

// handlers
export const LaunchRequestHandler = {
  canHandle(handlerInput: Alexa.HandlerInput) {
    return handlerInput.requestEnvelope.request.type == 'LaunchRequest';
  },

  handle(handlerInput: Alexa.HandlerInput) {
    const speechText = 'Welcome to the Alexa Skills Kit, you can say hello!';

    return handlerInput.responseBuilder
      .speak(speechText)
      .reprompt(speechText)
      .withSimpleCard('Hello World', speechText)
      .getResponse();
  }
};

export const HelloWorldIntentHandler = {
  canHandle(handlerInput: Alexa.HandlerInput) {
    return handlerInput.requestEnvelope.request.type === 'IntentRequest'
      && handlerInput.requestEnvelope.request.intent.name === 'HelloWorldIntent';
  },
  handle(handlerInput: Alexa.HandlerInput) {
    const speechText = 'Hello World!';

    return handlerInput.responseBuilder
      .speak(speechText)
      .withSimpleCard('Hello World', speechText)
      .getResponse();
  }
};

export const HelpIntentHandler = {
  canHandle(handlerInput: Alexa.HandlerInput) {
    return handlerInput.requestEnvelope.request.type === 'IntentRequest'
      && handlerInput.requestEnvelope.request.intent.name === 'AMAZON.HelpIntent';
  },
  handle(handlerInput: Alexa.HandlerInput) {
    const speechText = 'You can say hello to me!';

    return handlerInput.responseBuilder
      .speak(speechText)
      .reprompt(speechText)
      .withSimpleCard('Hello World', speechText)
      .getResponse();
  }
};

export const CancelAndStopIntentHandler = {
  canHandle(handlerInput: Alexa.HandlerInput) {
    return handlerInput.requestEnvelope.request.type === 'IntentRequest'
      && (handlerInput.requestEnvelope.request.intent.name === 'AMAZON.CancelIntent'
        || handlerInput.requestEnvelope.request.intent.name === 'AMAZON.StopIntent');
  },
  handle(handlerInput: Alexa.HandlerInput) {
    const speechText = 'Goodbye!';

    return handlerInput.responseBuilder
      .speak(speechText)
      .withSimpleCard('Hello World', speechText)
      .getResponse();
  }
};

export const SessionEndedRequestHandler = {
  canHandle(handlerInput: Alexa.HandlerInput) {
    return handlerInput.requestEnvelope.request.type === 'SessionEndedRequest';
  },
  handle(handlerInput: Alexa.HandlerInput) {
    //any cleanup logic goes here
    return handlerInput.responseBuilder.getResponse();
  }
};

export const ErrorHandler = {
  canHandle() {
    return true;
  },
  handle(handlerInput: Alexa.HandlerInput, error: any) {
    console.log(`Error handled: ${error.message}`);

    return handlerInput.responseBuilder
      .speak('Sorry, I can\'t understand the command. Please say again.')
      .reprompt('Sorry, I can\'t understand the command. Please say again.')
      .getResponse();
  },
};
//}

=== リクエストの検証とスキルのコードを実装


「AlexaスキルをWebサービス化する要件」の節で説明したように、AlexaをWebサービスで構築する場合には、受信したリクエストがAlexaデバイスのものであるかを検証することが必要です。検証には ASK SDK のアドオンである ask-sdk-express-adapter が利用できます。検証コードはスキルのリクエストを ask-sdk-express-adapter に渡したときに、ライブラリの中で自動的に検証してくれるので、開発時に検証コードを書く必要はありません。（ただし、ask-sdk-express-adapter のデフォルトで行う検証については懸念もあります。この点については、後述する 「リクエストの厳密な検証」の節で詳しく説明しています）@<br>{}

====== ask-sdk-express-adapter@<br>{}
@<href>{https://www.npmjs.com/package/ask-sdk-express-adapter}

//cmd[]{
yarn add ask-sdk-express-adapter
//}

インストールが終わっら、server.ts を次のように変更します。

//emlist[server.ts][typescript]{

import express from 'express'
import Skill from './skill' // 追加
import { ExpressAdapter } from 'ask-sdk-express-adapter' // 追加

const app = express()
const PORT = 80;
const HOST = '0.0.0.0';

// 追加ここから
const skill = Alexa.SkillBuilders.custom()
  .addRequestHandlers(
    Skill.LaunchRequestHandler,
    Skill.HelloWorldIntentHandler,
    Skill.HelpIntentHandler,
    Skill.CancelAndStopIntentHandler,
    Skill.SessionEndedRequestHandler,
  )
  .addErrorHandlers(Skill.ErrorHandler)
  .create()
// 追加ここまで

const adapter = new ExpressAdapter(skill, true, true); // 追加
// スキルのリクエスト
app.post('/', adapter.getRequestHandlers()); // 変更

// Application Load Balancer の ヘルスチェックを受けるために HTTP GET をリッスンしておく
app.get('/', (req: express.Request, res: express.Response) => {
  console.log(req)
  res.send('Hello World');
})

app.listen(PORT, HOST);
console.log(`Running on http://${HOST}:${PORT}`);

//}


変更が終わったら、コンテナをビルドしなおして、AWS ECR に Push しましょう。


//cmd[]{

# Dockerイメージをビルド
docker build -t haruharuharuby/alexa_fargate

# ローカルイメージを ECR に Push
$(aws ecr get-login --no-include-email --region us-east-1 --profile self)e

WARNING! Using --password via the CLI is insecure. Use --password-stdin.
Login Succeeded

# Amazon ECRのイメージにタグ付け
docker tag haruharuharuby/alexa_fargate:latest YOUR_AWS_ACCOUNT_ID.dkr.ecr.us-east-1.amazonaws.com/alexa-web-service:latest

# Amazon ECRに Push
docker push YOUR_AWS_ACCOUNT_ID.dkr.ecr.us-east-1.amazonaws.com/alexa-web-service:latest

# Pushされたことを確認
aws ecr list-images --repository-name alexa-web-service --region us-east-1 --profile self
{
    "imageIds": [
        {
            "imageDigest": "sha256:xxxxxxxx",
            "imageTag": "latest"
        }
    ]
}
//}


デプロイには数分かかります。
そして、先ほどデプロイしたエンドポイントを叩いてみましょう。


//cmd[]{
curl -X POST https://alexa.hugtech.io
AskSdk.Skill dispatch failed Error, Unexpected token o in JSON at position 1%  

curl -X GET https://alexa.hugtech.io
Hello World%
//}


最初に実行したリクエストの結果が変わりました。Alexaのリクエストの中身を全く送っていないので、この結果は期待通りです。



では、開発者コンソールで呼び出し元となるスキルを作って、'https://alexa.hugtech.io' を エンドポイントに設定してリクエストの検証がうまくいくか試してみましょう。


=== Alexaスキルを作る（開発者コンソール）


呼び出し元となるAlexaスキルは、Hello World を応答するシンプルなものです。
スキルのタイプは、カスタムスキルを選択します。
設定は次のとおりです。


//image[invocation][呼び出し名][border=on]{
//}

//image[endpoint][エンドポイント設定][border=on]{
//}

//image[intent][インテント][border=on]{
//}




開発者コンソールでテストしてみましょう。



//image[launchrequest][ローンチリクエスト][border=on]{
//}

//image[helloworldintent][HelloWorldIntent][border=on]{
//}




成功です。Webサーバーから正しくレスポンスが返ってきました！
ついに Alexa Webサーバーを立ち上げることができました！



おめでとうございます！


=== スキルの更新


スキルが立ち上がったあとのアップデートは簡単です。
server.ts をアップデートしたら、次の要領で最新のコンテナイメージを AWS ECR に Push するだけです。


//cmd[]{
# ローカルイメージを Amazon ECR に Push
$(aws ecr get-login --no-include-email --region us-east-1 --profile self)        
WARNING! Using --password via the CLI is insecure. Use --password-stdin.
Login Succeeded

# Amazon ECRのイメージにタグ付けして
docker tag haruharuharuby/alexa_fargate:latest YOUR_AWS_ACCOUNT_ID.dkr.ecr.us-east-1.amazonaws.com/alexa-web-service:latest

# Amazon ECRに Push
docker push YOUR_AWS_ACCOUNT_ID.dkr.ecr.us-east-1.amazonaws.com/alexa-web-service:latest
//}

=== リクエストの厳密な検証


本章では、リクエストの検証に ask-sdk-express-adapter を使いました。しかし、ask-sdk-express-adapter が持つデフォルトの検証機能は Alexaのドキュメントに記載されている全ての検証項目を行っているわけではありません。検証する証明書は３つの証明書で構成された証明書チェーン形式になっており、ask-sdk-express-adapter は 3つチェーンのうち最後に繋げられた証明書についてのみ検証を行っています。



公式ドキュメントの次の部分です。「Alexaから送信されたリクエストを手動で検証する」の 項番 3-C です。


//quote{
チェーンのすべての証明書が結合されてルートCA証明書の信頼チェーンが作られていることを検証します。
//}

@<href>{https://developer.amazon.com/ja-JP/docs/alexa/custom-skills/host-a-custom-skill-as-a-web-service.html#manually-verify-request-sent-by-alexa}
　@<br>{}
証明書チェーンの検証を含めた検証コードを次に示します


//emlist[verifier.ts][typescript]{

import axios from 'axios'
import crypto from 'crypto'
import forge from 'node-forge'
import { Verifier, } from 'ask-sdk-express-adapter'
import { IncomingHttpHeaders } from 'http'

const verifier: Verifier = {
  verify: verifyRequest
}

async function verifyRequest (requestEnvelope: string, headers?: IncomingHttpHeaders): Promise<void | string> {
    try {
      if (!headers) {
        return '[Error] Null headers'
      }

      // Step1: Verify Signature Certificate Chain Url has specific format.
      const regex = /https:\/\/s3\.amazonaws\.com(:443)*\/echo\.api\/(\.\.\/echo\.api\/)*echo-api-cert(-[0-9]*)\.pem/
      // @ts-ignore
      const isMatch = regex.test(headers.SignatureCertChainUrl)
      if (!isMatch) {
        return ''
      }

      // Step2: Download Certificate
      // @ts-ignore
      const pem = await axios.get(headers.SignatureCertChainUrl)

      // Step3: Check Certificate timestamp
      const pki = forge.pki
      const cert = pki.certificateFromPem(pem.data)
      cert.validity.notBefore
      const now = new Date().getTime()
      if (now < new Date(cert.validity.notBefore).getTime() || now > new Date(cert.validity.notAfter).getTime()) {
        return 'certificate expired' 
      }

      // Step4: Check SANS has specific name
      const sans = cert.getExtension('subjectAltName')
      // @ts-ignore
      if (!sans['altNames'].find((i):boolean => i.value === 'echo-api.amazon.com')) {
        return 'invalid subject alternative name' 
      }

      // Step5: Check all Certificate exactly combine all together
      const regex2 = /(-----BEGIN CERTIFICATE-----[^-]+-----END CERTIFICATE-----)/g
      const match = pem.data.match(regex2)

      let i = 0
      let validateChain = []
      for(i = 0; i < match.length-1; i++) {
        const c1 = pki.certificateFromPem(match[i])
        const c2 = pki.certificateFromPem(match[i+1])
        // @ts-ignore
        const result = c1.issuer.attributes.every(c1Item => {
          // @ts-ignore
          const r = c2.subject.attributes.find(c2Item => {
            return (c2Item.type === c1Item.type)
              && (c2Item.value === c1Item.value)
              && (c2Item.valueTagClass === c1Item.valueTagClass)
              && (c2Item.name === c1Item.name)
              && (c2Item.shortName === c1Item.shortName)
          })
          return r !== null
        })
        validateChain.push(result)
      }
      if (!validateChain.every(i => i === true)) {
        return 'No suitable chain' 
      }

      // Step6: Check to match hashes of Signature and body
      const verifier = crypto.createVerify('RSA-SHA1');
      verifier.update(requestEnvelope, 'utf8')
      // @ts-ignore
      if (!verifier.verify(pem.data, headers.Signature, 'base64')) {
        return 'hash mismatch' 
      }

      // Step7: Check time limit of each request
      const body = JSON.parse(requestEnvelope)
      if (new Date(body.request.timestamp).getTime() + 150 * 1000 < new Date().getTime() ) {
        return 'request timed out' 
      }
      return
    } catch (e) {
      return e.toString()
    }
  }

  export default verifier
//}


このスクリプトを verifier.ts というファイルで保存し、今回作成したスキルに埋め込んでみましょう。
ask-sdk-express-adapter は 独自の検証機能をインポートできるように設計されています。デフォルトの検証機能をOFFにして、先ほどのスクリプトでリクエストを検証するように変更したのが次のコードになります。


//emlist[server.ts][typescript]{

import express from 'express'
import Skill from './skill'
import { ExpressAdapter } from 'ask-sdk-express-adapter'
import verifier from './verifier' // 追加

const app = express()
const PORT = 80;
const HOST = '0.0.0.0';

const skill = Alexa.SkillBuilders.custom()
  .addRequestHandlers(
    Skill.LaunchRequestHandler,
    Skill.HelloWorldIntentHandler,
    Skill.HelpIntentHandler,
    Skill.CancelAndStopIntentHandler,
    Skill.SessionEndedRequestHandler,
  )
  .addErrorHandlers(Skill.ErrorHandler)
  .create()

const adapter = new ExpressAdapter(skill, false false, [verifier]); // 変更
// スキルのリクエスト
app.post('/', adapter.getRequestHandlers());

// Application Load Balancer の ヘルスチェックを受けるために HTTP GET をリッスンしておく
app.get('/', (req: express.Request, res: express.Response) => {
  console.log(req)
  res.send('Hello World');
})

app.listen(PORT, HOST);
console.log(`Running on http://${HOST}:${PORT}`);

//}


Webサービスとして外部に公開しているわけですので、証明書チェーンまで厳密に検証を行うほうがセキュリティとしてはベターです。


== おわりに


いかがだったでしょうか。普段 Hosted Skill や Voiceflow などのビジュアルツール を使うスキル開発者には難しい内容になってしまったのではと思います。しかし、この構成ができてしまえば、Dockerコンテナを us-east-1, ap-northeast-1 など、別のリージョンへのデプロイも簡単になります。スキルがスケールしたときに、さっとコンテナをデプロイして、別の地域でサービス提供なんていうこともできます。コンテナに柔軟にタグをうち、タグごとにデプロイをして、Route 53のエンドポイントで向き先を簡単に変えることもできるようになります。

Webサービスになって公開した分、別の開発者、開発会社さんに会話モデルを作ってもらって、エンドポントを自社のサーバーに向けてもらう、なんていうこともしやすくなるかもしれません。いきなり違う言語で作り変える、ということもできてしまいます。最初の手間は確かにありますが、ひとつ頭の片隅にこんな方法もあるのだとおいてもらえればと思います。今後のスキル開発のエッセンスになれば幸いです。

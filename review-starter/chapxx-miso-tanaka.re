
= TypeScript × VS Code × Alexaスキル開発


//image[tag][]{
//}



//flushright{
著者：田中 みそ（@<href>{https://twitter.com/miso_develop/,@miso_develop}）
//}


== はじめに


本章では、昨今波に乗りまくっているTypeScriptを用いてAlexaスキルを超効率爆速開発できるようになるため、JavaScriptで書かれたAlexaスキルを移植しながらTypeScriptを身に着けていきます。@<br>{}
なお本章では@<tt>{ngrok}を用いてローカルにサーバーをたて、Alexa Skills Kit開発者コンソールのテスト画面から動作確認を行うため、クラウド環境や実機は必要ありません。  


===== 本章の対象読者@<br>{}


本章の対象読者は、次のような方です。  

 * TypeScriptが気になっているけどまだ触ったことがない
 * @<tt>{tsconfig.json}を開いてそっ閉じしてしまった
 * Alexaスキル開発をもっと効率的に行いたい


===== 本章の前提知識@<br>{}


本章を読み進めるにあたり、次の基礎知識が必要です。  

 * JavaScript
 * Alexaスキル開発
 * Visual Studio Code


===== 本章の必要環境@<br>{}


本章を読み進めるにあたり、次の環境が必要です。  

 * Visual Studio Code
 * Node.js v12以上
 * Git


== TypeScriptについて


この節ではTypeScriptの特徴、メリットについて説明します。  


=== TypeScriptとは


TypeScriptには次のような特徴があります。  

 * Microsoftが開発したオープンソースのプログラミング言語
 * 省略可能な静的型付け
 * クラスベースのオブジェクト指向
 * JavaScriptと互換性をもつスーパーセット
 * TypeScriptのソースからJavaScriptのソースを生成する


=== TypeScriptのメリット


主に次のようなメリットがあります。  

 * 型
 * 入力補完
 * 最新のECMAScript構文を使える
 * JavaScript完全互換



これらについて詳しく説明します。  


=== 型

==== いにしえの型


CやJavaのようないにしえの静的型付き言語では、基本的にはメモリーを確保するために型が使われていました。たとえば、Javaでは数値型だけでも複数の型があり、メモリーの確保量が異なります。  

 * byte（8bit）
 * short（16bit）
 * int（32bit）
 * long（64bit）
 * float（32bit浮動小数型）
 * double（64bit浮動小数型）


==== ナウい型


対してJavaScriptやTypeScriptの型は、ナウでヤングな型になっています。@<br>{}
数値ならnumber型のみ、文字列ならstring型のみであり、型のメモリーへの影響はありません。  



いにしえの型はコンピューターのために存在していましたが、ナウい型はソースを書く人間のために存在しています。  


==== JavaScriptの型


JavaScriptでは表面的には型はありませんが、内部的には型が存在しています。@<br>{}
次のような、@<tt>{typeof xxx}で返ってくる値が内部的な型です。  

 * プリミティブ型
 ** boolean（真偽値）
 ** string（文字列）
 ** number（数値）
 ** undefined
 ** null
 * オブジェクト型
 ** object（オブジェクト、配列）
 ** Function（関数）


==== TypeScriptの型


TypeScriptの型はJavaScriptの型に加え、どんな型の値でも入るany、unknownのほか、リテラル型、タプル型、ユニオン型と、さまざまな型があります。@<br>{}
それらを組み合わせたりクラスを作ったりと、自由自在かつ柔軟に型を定義できます。  



//image[type][TypeScriptのさまざまな型]{
//}



==== 型の関係


型のかたさの関係は、次の図のようになります。色が濃いほどかたく（厳格に）なります。  



//image[typelist][型の関係 ※簡略化のため一部の型のみ抜粋しています]{
//}



==== anyとunknown@<br>{}

===== any@<br>{}


どんな型の値でも入れられる型（JavaScriptと同じ状態）です。@<br>{}
ただし型チェックや入力補完といった恩恵が得られなくなってしまいます。@<br>{}
そういった恩恵がなくてもいいから、とりあえず書き進めたいときなどに使います。  



//image[any][any型の例]{
//}



===== unknown@<br>{}


anyでは危なっかしいということで、ver3.0にて追加された型です。@<br>{}
any同様どんな型の値でも入りますが、使うときは何の型か分かる状況でないと使えません。@<br>{}
どういった状況かというと、次の図のように型アサーションで型を明示するか、ifとtypeofにより何の型か分かりきっている状況です。@<br>{}
安全に書き進めたいなら、anyではなくunknownを使用するとよいでしょう。  



//image[unknown][unknown型の例]{
//}



==== 代入警告


誤った型の値を代入したときに警告してくれます。@<br>{}
これによりコードを書いているときに気付くことができ、不毛なデバッグを回避できます。  



//image[type-alert][代入警告の例]{
//}



==== 型推論


分かりきっている型を推論してくれます。@<br>{}
次の図では変数numに対して型を定義していませんが、代入した値が数値型であるため、TypeScriptがnumは数値型だと推論してくれています。@<br>{}
ifとtypeofにより型が絞られた状況でも、同様に型推論を行ってくれます。  



//image[type-inference][型推論の例]{
//}



=== 入力補完


型が分かっているオブジェクトのメソッドやプロパティーを補完できます。@<br>{}
情報がツールチップに表示されるので、ドキュメントを確認せずに済みます。@<br>{}
ライブラリや誰かが作ったAPIでも、型定義があれば引数や戻り値が分かります。  



//image[completion][入力補完の例]{
//}



=== 最新のECMAScript構文を使える


最新ECMAScriptで書いたTypeScriptのソース（@<tt>{.ts}）から、任意のバージョンのJavaScriptのソース（@<tt>{.js}）へトランスパイルされるので、FaaS等デプロイ先のNode.js環境が古くても最新の構文が使えます。@<br>{}
ただしサポートされるのはあくまで構文です。@<br>{}
追加メソッドも使いたい場合は、別途ポリフィルを入れる必要があります。  


==== ver3.7.2で導入された@<tt>{Optional Chaining}と@<tt>{Nullish Coalescing}


また、TypeScript ver3.7.2では@<tt>{Optional Chaining}と@<tt>{Nullish Coalescing}が導入されました。@<br>{}
この2つの構文は、ver3.7.2リリース時点ではECMAScriptの仕様でもStage3@<fn>{annotation1}の状態で、リリースまではされていません。@<br>{}
そういった構文も、TypeScriptの実装状況によってはいち早く使うこともできます。  



ちなみに、@<tt>{Optional Chaining}を使うとAlexaでたまに見るような次のコードを、とても簡潔に書くことができます。  


//emlist{
// JavaScript
request
&& request.context
&& request.context.System
&& request.context.System.device
&& request.context.System.device.supportedInterfaces
&& request.context.System.device.supportedInterfaces['Alexa.Presentation.APL']

// TypeScript ver3.7.2
request?.context?.System?.device?.supportedInterfaces['Alexa.Presentation.APL']
//}


//footnote[annotation1][Stage0～Stage4までの5段階中、4段階目で、仕様まで決まってはいるもののまだ実装されていない状態]


=== JavaScript完全互換


TypeScriptはJavaScriptを完全に含みます。@<br>{}
そのため、@<tt>{tsconfig.json}で@<tt>{strict}をオフにしてしまえば、ソースの中身を変えずとも拡張子を@<tt>{.ts}に変更するだけで使えます。  


=== 読んでおいたほうがいいもの


次のドキュメントは、目を通すことをおすすめします。  

 * @<href>{http://js.studio-kingdom.com/typescript/,公式ドキュメント日本語訳}@<fn>{url2}
 * @<href>{https://typescript-jp.gitbook.io/deep-dive/,TypeScript Deep Dive}@<fn>{url3}



//footnote[url1][@<href>{http://www.typescriptlang.org/docs/tutorial.html}]
//footnote[url2][@<href>{http://js.studio-kingdom.com/typescript/}]
//footnote[url3][@<href>{https://typescript-jp.gitbook.io/deep-dive/}]



//clearpage


== JavaScriptサンプルスキルの構築


この節ではTypeScriptへの移植元となるJavaScriptのスキルを構築します。@<br>{}
@<tt>{ngrok}を用いてローカルにサーバーをたて、Alexa Skills Kit開発者コンソールのテスト画面で動作確認します。  


=== サンプルスキル


サンプルスキルとして、Alexaに伝えた数のサイコロを振り、合計の数を答えるスキルを用意しています。@<br>{}
スキルのソースはGitHubに公開していますので、リポジトリーを@<tt>{git clone}し、サンプルスキルをローカル環境に構築します。@<br>{}
@<tt>{git clone}後、サンプルスキルのディレクトリに移動し、Visual Studio Code（以下、VS Code）を起動します。@<br>{}
※Macでcodeコマンドを使用する場合、VS Codeのパスを通しておく必要があります


//cmd[]{
git clone https://github.com/miso-develop/typescript-vscode-alexa
cd typescript-vscode-alexa/start/
code .
//}


//note[本章を最後まで終えた段階のソース]{
「@<tt>{typescript-vscode-alexa/complete/}」が、本章を最後まで終えた段階のソースとなります。@<br>{}
本章を読み進めていく中でお手元の環境でうまく動作しなかった場合は、こちらのディレクトリのソースと見比べてみてください。
//}


=== npmパッケージのインストールとNode実行


@<tt>{ask-sdk}や@<tt>{express}といった、スキルに必要なパッケージのインストールと、プログラムの実行を行います。@<br>{}
VS Codeにて@<tt>{Ctrl + @ (⌘ + ⇧ + @)}のショートカットを実行してターミナルパネルを開き、次のコマンドを実行します。@<br>{}
@<strong>{※ここから先のコマンドはすべてVS Code上のターミナルで実行します}  


//cmd[]{
npm ci
node index.js
//}


ターミナルに「@<tt>{start!}」と表示されたら正常に実行できています。  



//clearpage


=== スキルの作成


@<href>{https://developer.amazon.com/alexa/console/ask,Alexa Skills Kit開発者コンソール}@<fn>{url4}を開き、カスタムスキルを作成します。@<br>{}
作成したスキルはテスト画面からしか起動しないため、スキル名は「サンプルダイス」など、既存のスキル名と重複しないスキル名であれば問題ありません。@<br>{}
　@<br>{}
スキルを作成したら「JSONエディター」を開き、@<tt>{git clone}したディレクトリ内の@<tt>{model.json}をアップロードします。@<br>{}
「モデルを保存」と「モデルをビルド」も忘れずに実行してください。  



//footnote[url4][@<href>{https://developer.amazon.com/alexa/console/ask}]


=== @<tt>{ngrok}のインストールと実行


@<tt>{ngrok}とは、ローカルで実行しているプログラムを、外部からhttpアクセスするためのトンネリングアプリケーションです。@<br>{}
一般的にスマートスピーカースキルでは、AWS LambdaやAzure FunctionsといったFaaS（Function as a Service）にプログラムをデプロイします。@<br>{}
開発中、コードを更新するたびにデプロイを行うことは手間ですし、デプロイの反映やログの確認にも若干のタイムラグが生じます。@<br>{}
　@<br>{}
そのため開発中は@<tt>{ngrok}を使いローカルにサーバーをたてることで、デプロイの手間を省き迅速に開発を進めることができます。@<br>{}
また、ログがすぐに確認できるメリットもあります。@<br>{}
　@<br>{}
VS Codeで新しいターミナルを開き次のコマンドを実行し（Nodeを実行しているターミナルはそのまま残しておいてください）、@<tt>{ngrok}のインストール、実行を行います。@<br>{}
なお、@<tt>{ngrok}は常に起動し続けている必要があるので、ターミナルを閉じないよう気を付けてください。  


//cmd[]{
npm install -D ngrok
npx ngrok http 3000
//}


そして、@<tt>{ngrok}起動時にターミナルへ表示される情報から@<tt>{Forwarding}のhttpsプロトコルURLをコピーします。  


//cmd[]{
ngrok by @inconshreveable                                       (Ctrl+C to quit)

Session Status                online
Account                       Miso Tanaka (Plan: Free)
Version                       2.2.8
Region                        United States (us)
Web Interface                 http://127.0.0.1:4040
Forwarding                    http://xxxxxxxx.ngrok.io -> localhost:3000
Forwarding                    @<cursor>{https://xxxxxxxx.ngrok.io} -> localhost:3000
//}

=== エンドポイントの設定


ブラウザに戻り、Alexa Skills Kit開発者コンソールよりエンドポイント画面を開きます。@<br>{}
次の手順でエンドポイントを設定します。  

 1. 「サービスのエンドポイントの種類」にて「HTTPS」を選択
 1. 「デフォルトの地域」に@<tt>{ngrok}のURLを入力
 1. 「SSL証明書の種類を選択」とあるセレクトボックスにて「開発用のエンドポイントは、照明機関が発行したワイルドカード証明書をもつドメインのサブドメインです」を選択
 1. 「エンドポイントを保存」ボタンをクリック



//image[endpoint][サーバー設定画面]{
//}



=== テストの実行


続いて、スキルのテストを行います。@<br>{}
Alexa Skills Kit開発者コンソールのテスト画面を開きます。@<br>{}
次の手順でテストを行います。  

 1. 画面左上のセレクトボックスにて「開発中」を選択
 1. 「サンプルダイス」と発話を入力
 1. スキルが起動したら、適当な数字を入力
 1. 結果が返ってくればOK



//image[test][スキルのテスト]{
//}




以降の手順でも何度かテストを行いますが、テスト結果が問題なければターミナルで実行したNodeプロセスを都度落としてください。  


=== ソースコードについて


ソースコードについて少し解説します。@<br>{}
VS Codeで@<tt>{index.js}を開いてみてください。@<br>{}
次のような構成になっています。  

 * 定数、ロジック、ハンドラーでブロックを分けている
 * 特別な実装は@<tt>{throwDiceHandler}ぐらい
 ** Alexaに伝えた数値分、サイコロを降って結果を発話する
 ** このあたりの内部処理は、ロジック部の3つの関数で処理している
 *** @<tt>{getThrowResultMessage}関数：発話した数値をもとに、以下の関数を実行し結果の発話を返す
 *** @<tt>{throwDice}関数：発話した数値分、サイコロを振った結果を返す
 *** @<tt>{resultText}関数：サイコロを振った結果をもとに、発話を組み立てて返す



//clearpage


=={typescript-setup} TypeScript環境構築


この節ではTypeScriptの環境を構築します。@<br>{}
ディレクトリ構成や関連パッケージ、TypeScriptの設定ファイルである@<tt>{tsconfig.json}について説明します。  


=== ディレクトリの作成とソース移動


@<tt>{src/}にTypeScriptのソース（@<tt>{.ts}）を格納し、@<tt>{dist/}にコンパイルされたJavaScriptのソース（@<tt>{.js}）を出力するようにします。@<br>{}
次のコマンドを実行し、ディレクトリの作成とソースファイルの移動を行います。@<br>{}
※@<tt>{mv}時に拡張子を@<tt>{js}から@<tt>{ts}に変える  


//cmd[]{
mkdir src dist
mv index.js src/index.ts
//}

=== TypeScriptとtypesyncのインストール


次のコマンドを実行し、TypeScript関連パッケージをインストールします。  


//cmd[]{
npm install -D typescript typesync
//}

=== 型定義のインストール


@<tt>{package.json}を開いたうえで、次のコマンドを実行してみてください。  


//cmd[]{
$ npx typesync
>>  TypeSync v0.6.1
    1 new typings added.

sample-dice — package.json (1 new typings added, 0 unused typings removed)
└─ + @types/express

    Go ahead and run npm install or yarn to install the packages that were added.
//}


@<tt>{typesync}コマンドにより、@<tt>{package.json}の@<tt>{devDependencies}に@<tt>{@types/express}というパッケージが追加されます。@<br>{}
このパッケージが型定義です。@<br>{}
この状態で@<tt>{npm install}を行い、型定義をインストールします。  


//cmd[]{
npm install
//}


//image[at-types][typesync実行後のpackage.json]{
//}



==== 型定義とは


世の既存ライブラリのほとんどはJavaScriptで作られているため、それらは型情報をもちません。@<br>{}
そのため、TypeScriptではせっかく型チェックや入力補完があるのに使えません。@<br>{}
そこでライブラリに対応した型定義だけを用意して読み込めば、ライブラリ自体のソースにも影響ないよね、ということで作られたのが型定義ファイルです。@<br>{}
@<tt>{xxx.d.ts}というファイル名のものです。  



//note[@typesとは]{
昔は@<tt>{tsd}という巨大な型定義リポジトリーを落としたり、@<tt>{typings}といった型定義専用のパッケージマネージャーのようなものがありました。@<br>{}
今では@<tt>{@types}というnpmパッケージ群から、簡単に型定義をインストールできるようになりました。@<br>{}
使っているパッケージ自体に型定義がなかったら、ひとまず@<tt>{npm install -D @types/xxx}としてみてください。
//}


=== TypeScriptの初期化


次のコマンドでTypeScriptの初期化を行い、@<tt>{tsconfig.json}を作成します。@<br>{}
@<tt>{tsconfig.json}が作成されたらVS Codeで開きます。  


//cmd[]{
npx tsc --init
//}

=== @<tt>{tsconfig.json}の編集


@<tt>{tsconfig.json}を編集し、ディレクトリ構成を設定します。@<br>{}
次の変更部のみ書き換えます。  


==== アウトプットディレクトリの指定

//emlist[tsconfig.json（15行目あたり）]{
- //"outDir": "./",
+ "outDir": "./dist",
//}

==== インプットディレクトリの指定

//emlist[tsconfig.json（65行目あたり）]{
- }
+ },
+ "include": ["./src/**/*"]
//}

=== @<tt>{tsconfig.json}について


@<tt>{tsconfig.json}のパラメーターについて、スキル開発にあたり主だった部分を抜粋して説明します。  


==== Basic Options
 * target：コンパイルされるECMAScriptバージョンを指定
 * lib：ECMAScriptの上位バージョンやDOMといった標準APIを有効化
 * sourceMap：Source Mapファイルを生成するか
 ** Source Mapファイルとは
 *** コンパイルされたJavaScriptの行がTypeScriptのどの行に相当するかを、デバッガーに伝えるためのファイル
 *** @<tt>{xxx.js.map}といった名前で出力される
 * outDir：コンパイルされた@<tt>{.js}の出力先ディレクトリを指定


==== Strict Type-Checking Options
 * strict：以下のすべての型チェックを有効化する
 * noImplicitAny：暗黙のanyを許可しない
 * strictNullChecks：厳密なnullチェック
 * strictFunctionTypes：関数の引数の厳密に型チェック
 * strictBindCallApply：bind、call、applyで厳密に型チェック
 * strictPropertyInitialization：クラスのプロパティー初期化をチェック
 * noImplicitThis：thisの型を指定しないとエラー
 * alwaysStrict："use strict"を有効化


==== Additional Checks
 * noUnusedLocals：未使用のローカル変数があるとエラー
 * noUnusedParameters：未使用のパラメーターがあるとエラー
 * noImplicitReturns：関数内に通らないコードがあるとエラー
 * noFallthroughCasesInSwitch：switch文でbreakがないとエラー


=== @<tt>{index.ts}の確認


ここで@<tt>{index.ts}を開き、@<tt>{Ctrl (⌘) + Shift (⇧) + m}を押しVS Codeの問題パネルを開いてみてください。@<br>{}
次の図のようにいくつかエラーが出ています。@<br>{}
これらは@<tt>{tsconfig.json}が作成されたことにより、TypeScriptのチェックがVS Code上で走り検出されたエラーです。  



//image[error][TypeScriptエラー]{
//}



=== @<tt>{strict}の無効化


@<tt>{tsconfig.json}にて、@<tt>{strict}をコメントアウトします。  


//emlist[tsconfig.json（26行目あたり）]{
- "strict": true,
+ // "strict": true,
//}


再度@<tt>{index.ts}のエラーをチェックすると、すべてのエラーが消えていることでしょう。@<br>{}
@<tt>{strict}を無効にしたことにより、すべての型チェックが無効化され、JavaScriptとほぼ同様の状態となりました。  


=== コンパイル


次のコマンドを実行し、コンパイルを行います。  


//cmd[]{
npx tsc
//}


@<tt>{dist/}配下に@<tt>{index.js}が生成されているので、これを開いてみましょう。@<br>{}
今回@<tt>{tsconfig.json}の@<tt>{target}にて@<tt>{es5}が指定された状態でコンパイルされているため、@<br>{}ECMAScript5形式のJavaScriptソースが出力されています。  


=== Node実行とテスト


この状態でプログラムを実行します。  


//cmd[]{
node dist/index.js
//}


ブラウザに戻り、@<tt>{テスト画面}からスキルをテストしてみてください。@<br>{}
JavaScriptの状態のときと同じ動きになります。@<br>{}
　@<br>{}
このようにJavaScriptのソースをそのまま持ってきても、@<tt>{tsconfig.json}の@<tt>{strict}を無効化しておくことで、コンパイルを実行してソースを動かすことができます。@<br>{}
この状態でもJavaScriptよりは若干チェックが厳しいのですが、TypeScriptの利点がほとんどない状態です。@<br>{}
　@<br>{}
たとえば、次のコードはJavaScriptではエラーになりませんが、TypeScriptではエラーとなります。  


//emlist{
"aaa"++ // JavaScriptだったらNaNで通るけど、TypeScriptだとエラー
//}

=== @<tt>{strict}を戻す


@<tt>{tsconfig.json}にて、@<tt>{strict}のコメントアウトを外して元に戻します。@<br>{}
型チェックのもっとも厳しい状態でTypeScriptへの移植を進めていきます。  


//emlist[tsconfig.json（26行目あたり）]{
- // "strict": true,
+ "strict": true,
//}


//note[型チェックの厳しさは必要に応じて]{
実際に開発を行う際は、必要に応じて@<tt>{strict}による型チェックの厳しさを調節しましょう。@<br>{}
特に@<tt>{noImplicitAny}はエラーが多発するので、無効化してしまうことも多いです。
//}



//clearpage


== JavaScriptからTypeScriptへの移植


この節ではJavaScriptのソースをTypeScriptへ書き換えつつ、TypeScriptのメリットを体感していきます。  


=== requireをimportに書き換え


まずは、@<tt>{require}を@<tt>{import}文に書き換えます。@<br>{}
TypeScriptはECMAScriptがベースとなっていますので、Node.jsのモジュール方式である「@<tt>{CommonJS}」ではなく、ECMAScriptの標準仕様である「@<tt>{ESModules}」で記述しましょう。@<br>{}
@<tt>{import}文でないとモジュールの型情報が読み込まれません。  


//emlist[index.ts（1行目）]{
- const Alexa = require("ask-sdk-core")
- const Model = require("ask-sdk-model")
- const { ExpressAdapter } = require("ask-sdk-express-adapter")
- const express = require("express")
+ import * as Alexa from "ask-sdk-core"
+ import * as Model from "ask-sdk-model"
+ import { ExpressAdapter } from "ask-sdk-express-adapter"
+ import express from "express"
//}

=== エラーを確認


VS Codeの問題パネルからエラーを確認してみましょう。@<br>{}
関数の引数に対する暗黙的anyのエラーが発生しています。  



//image[error][暗黙的anyのエラー]{
//}




暗黙的anyのエラーとは、TypeScriptが型推論できない箇所に暗黙的にany型を割り当てることに対するエラーです。@<br>{}
明示的にanyを指定することでもエラーは解消されますが、ここでは正しい型を割り当てていきましょう。  


=== @<tt>{diceCount}の型


名前から推測できるとおり、数値型が正解です。@<br>{}
次のようにnumber型で型アノテーションをつけましょう。@<br>{}
修正すべき箇所が2ヵ所あるので、両方ともソースを修正します。@<br>{}
これにより@<tt>{diceCount}の暗黙的anyのエラーは解消されます。  


//emlist[index.ts（25行目）]{
- const getThrowResultMessage = (diceCount) => {
+ const getThrowResultMessage = (diceCount: number) => {
//}

//emlist[index.ts（34行目）]{
- const throwDice = (diceCount) => {
+ const throwDice = (diceCount: number) => {
//}

=== 新たに発生したエラー


@<tt>{diceCount}の暗黙的anyのエラーは解消されたものの、今度は96行目の@<tt>{const throwResultMessage = getThrowResultMessage(diceCount)}で新たにエラーが発生しました。@<br>{}
@<tt>{diceCount}にnumber型を指定したことにより、より具体的な型推論が走ったため生じています。@<br>{}
エラーの内容を確認してみましょう。  



//image[new-error][新たに発生したエラー]{
//}



//emlist[getThrowResultMessage(diceCount)のエラー内容]{
型 'string | 1' の引数を型 'number' のパラメーターに割り当てることはできません。
  型 'string' を型 'number' に割り当てることはできません。ts(2345)
//}


@<tt>{getThrowResultMessage}の引数である@<tt>{diceCount}の型はnumber型ですので、string型は入れられない、というエラーです。@<br>{}
@<tt>{diceCount}への代入を行っている1つ上の行を見てみましょう。@<br>{}
@<tt>{handlerInput.getSlots().number || 1}と、スロットの数値を代入しており、一見number型を格納していそうです。  


=== スロットの値は文字列型


しかし、実はAlexaのスロットの値は文字列型です。@<br>{}
スキルでAlexaに数字を伝えたつもりでも、エンドポイントに届く情報はJSONデータです。@<br>{}
それがパースされると数値型ではなく、文字列型となります。@<br>{}
これはスロットの扱いでわりとハマりやすいポイントでしょう。  



//image[slot][エンドポイントに届くスロット情報]{
//}




もしJavaScriptで開発していた場合、実装時点ではこのエラーが発生しません。@<br>{}
スキルが実行されたときにエラーが発生するまで、バグが潜んだままとなります。@<br>{}
　@<br>{}
ひととおり実装を終えたあとでエラーが発生しても、原因の特定から始めなければならないため、デバッグに余計な時間を費やすことになります。@<br>{}
しかし、TypeScriptであれば型チェックにより、こうして未然にエラーを検出してくれます。  


=== 型エラーの対処


エラーの原因が判明したので、対処していきましょう。@<br>{}
必ず数値型とすべきですので、次のようにnumber型として型アサーションをつけることでエラーを解消します。  


//emlist[index.ts（93行目）]{
- const diceCount = Alexa.getSlotValue(handlerInput.requestEnvelope, "number") || 1
+ const diceCount = (Alexa.getSlotValue(handlerInput.requestEnvelope, "number") || 1) as number
//}

=== @<tt>{handlerInput}の型


ぱっと見、どんな型なのかよく分からないのでany型としておきたいところですが、このオブジェクトはスキル開発時によく使うのでちゃんと型を指定しておきましょう。@<br>{}
　@<br>{}
使用されているのは各ハンドラーオブジェクトの@<tt>{canHandle}、@<tt>{handle}メソッドです。@<br>{}
各ハンドラーオブジェクトを引数にとっている@<tt>{addRequestHandlers}までたどり（139行目）、メソッド名をマウスオーバーします。@<br>{}
次の図のようにツールチップが表示されます。  



//image[tooltip][onLaunchRequestのツールチップ]{
//}



//emlist[onLaunchRequestのツールチップ]{
(method) BaseSkillBuilder.addRequestHandlers(...requestHandlers: Alexa.RequestHandler[]): Alexa.CustomSkillBuilder
//}


@<tt>{...requestHandlers: Alexa.RequestHandler}とあるように、各ハンドラーオブジェクトは@<tt>{Alexa.RequestHandler}型ということがわかります。@<br>{}
では、その配下にある@<tt>{canHandle}と@<tt>{handle}メソッドの引数である@<tt>{handlerInput}はどんな型なのか。@<br>{}
ASK SDKのソースを掘り下げて読み解いていきましょう。@<br>{}
　@<br>{}
@<tt>{addRequestHandlers}をCtrl（⌘）キーを押しながらクリックします。@<br>{}
すると、@<tt>{addRequestHandlers}が定義されている@<tt>{BaseSkillBuilder}というインターフェースの型定義ファイルが開かれます。@<br>{}
VS Codeではこのように、オブジェクトやメソッドなどを@<tt>{Ctrl (⌘) + クリック}するとその定義部へジャンプできます。  



//image[BaseSkillBuilder-type][BaseSkillBuilderインターフェース]{
//}




新たに開かれた型定義ファイルにて、@<tt>{addRequestHandlers(...requestHandlers: CustomSkillRequestHandler[]): this;}という行がフォーカスされています。@<br>{}
これが@<tt>{addRequestHandlers}の型定義となります。@<br>{}
　@<br>{}
続いて、@<tt>{requestHandlers}の型である@<tt>{CustomSkillRequestHandler}を@<tt>{Ctrl (⌘) + クリック}してみましょう。@<br>{}
新たに@<tt>{CustomSkillRequestHandler}インターフェースの型定義ファイルが開かれました。  



//image[CustomSkillRequestHandler-type][CustomSkillRequestHandlerインターフェース]{
//}




@<tt>{export interface CustomSkillRequestHandler extends RequestHandler<HandlerInput, Response> {\}}と、@<tt>{RequestHandler}インターフェースを拡張していることが分かりますので、さらに@<tt>{RequestHandler}を@<tt>{Ctrl (⌘) + クリック}してみましょう。@<br>{}
@<tt>{RequestHandler}インターフェースの型定義ファイルが開かれます。  



//image[RequestHandler-type][RequestHandlerインターフェース]{
//}




ここまで来ると、見慣れた@<tt>{canHandle}と@<tt>{handle}メソッドが現れます。@<br>{}
さっそく引数の型を見てみると@<tt>{input: Input}とあります。@<br>{}
この@<tt>{Input}型はなんなのかというと、インターフェース定義部の@<tt>{RequestHandler<Input, Output>}にあります。@<br>{}
@<tt>{<Input, Output>}という記述は、TypeScriptの型引数の構文となり、@<tt>{RequestHandler}が呼び出された際に引数部の@<tt>{Input}に指定された型が、@<tt>{canHandle}、@<tt>{handle}メソッドの引数の型となります。@<br>{}
　@<br>{}
@<tt>{RequestHandler}型を拡張していた@<tt>{CustomSkillRequestHandler}インターフェースの型定義へ戻ってみましょう。  



//image[CustomSkillRequestHandler-type][CustomSkillRequestHandlerインターフェース]{
//}




@<tt>{RequestHandler<HandlerInput, Response>}とあり、最初の引数である@<tt>{HandlerInput}が@<tt>{handlerInput}の型ということが分かります。  


=== @<tt>{HandlerInput}型アノテーションをつける


@<tt>{handlerInput}の型は@<tt>{HandlerInput}型だと判明したので（そのまんまですね…）、型付けを行いTypeScriptのエラーを解消していきましょう。@<br>{}
　@<br>{}
変数や関数の引数、戻り値に明示的に型を指定する記述を、型アノテーションと呼びます。@<br>{}
型アノテーションは変数や関数の引数は名前の後ろに@<tt>{: 型名}という形式で記述します。@<br>{}
関数の戻り値の場合は引数部の@<tt>{()}の後ろに@<tt>{: 型名}という形式で記述します。  


//emlist[型アノテーションの例]{
const name: string = "alexa"
const getAge = (name: string): number => { return user[name].age }
const age: number = getAge(name)
//}


それでは、@<tt>{handlerInput}に@<tt>{HandlerInput}型アノテーションをつけてみましょう。  


//emlist[index.ts（70行目）]{
- canHandle(handlerInput) {
+ canHandle(handlerInput: HandlerInput) {
//}


今度は、追記した@<tt>{HandlerInput}でエラーが発生しました。@<br>{}
エラー箇所をマウスオーバーしツールチップを確認すると、@<tt>{HandlerInput}型が見つからないというエラーのようです。  


//emlist[HandlerInputのエラー]{
名前 'HandlerInput' が見つかりません。ts(2304)
//}

=== @<tt>{HandlerInput}型を探す


@<tt>{HandlerInput}型がどこで定義されているか探してみましょう。@<br>{}
ASK SDKのどこかにあるはずですので、ひとまず型定義を見てみます。@<br>{}
　@<br>{}
1行目のimport文から@<tt>{ask-sdk-core}を@<tt>{Ctrl (⌘) + クリック}して、型定義を開きましょう。@<br>{}
すると、型定義の6行目で@<tt>{HandlerInput}がexportされています。@<br>{}
つまり、現在importによって定義されている@<tt>{Alexa}オブジェクトの直下に存在しているということです。  



//image[sdk-d-ts][ASK SDKの型定義]{
//}



=== 型アノテーションの修正


先ほど@<tt>{HandlerInput}型アノテーションを追加した箇所を次のように修正します。@<br>{}
これでエラーが解消されます。  


//emlist[index.ts（1行目）]{
- canHandle(handlerInput: HandlerInput) {
+ canHandle(handlerInput: Alexa.HandlerInput) {
//}

=== 入力補完を試してみよう


ここまでで@<tt>{handlerInput}に@<tt>{HandlerInput}型が割り当てられました。@<br>{}
これにより@<tt>{handlerInput}に対して入力補完が効くようになりましたので、試してみましょう。@<br>{}
　@<br>{}
先ほど@<tt>{HandlerInput}型アノテーションを付与した@<tt>{launchHandler}の@<tt>{canHandle}メソッド内で、@<tt>{handlerInput.}と入力します。@<br>{}
最後の@<tt>{.}を入力したタイミングでサジェストが表示され、@<tt>{HandlerInput}オブジェクトのメンバー一覧が表示されます。@<br>{}
　@<br>{}
次の図のように、矢印キーで@<tt>{requestEnvelope}を選択します。@<br>{}
すると右側のツールチップに@<tt>{(property) HandlerInput.requestEnvelope: Model.RequestEnvelope}と表示されます。@<br>{}
表示内容から、@<tt>{requestEnvelope}プロパティーは@<tt>{Model.RequestEnvelope}型だということが分かります。  



//image[suggestion][handlerInputのサジェスト]{
//}




Enterキーを押し@<tt>{requestEnvelope}を確定させ、さらに@<tt>{.}を入力してみましょう。@<br>{}
@<tt>{RequestEnvelope}のメンバー一覧が表示されますので、今度は@<tt>{request}を選択します。@<br>{}
@<tt>{request}も@<tt>{Request}型であるため、さらにメンバーが存在します。@<br>{}
再度@<tt>{.}を入力し、候補の中から@<tt>{type}を選択してみましょう。@<br>{}
　@<br>{}
@<tt>{handlerInput.requestEnvelope.request.type}というコードとなりました。@<br>{}
これはお馴染みの@<tt>{LaunchRequest}や@<tt>{IntentRequest}といったリクエストタイプが格納されるプロパティーです。@<br>{}
@<tt>{canHandle}メソッド内でよく使われる値ですね。@<br>{}
　@<br>{}
このプロパティーはイチからタイピングしていくと中々しんどいですが、入力補完を使うことでこのように少ない入力で記述できます。@<br>{}
また、Alexaの仕様をなんとなくでも覚えていれば、入力補完でオブジェクトのメンバーを適当にたどっていくだけでも仕様を見返すことなくコーディングできます。@<br>{}
　@<br>{}
さらに、型定義によってはメソッドやプロパティーの詳細な説明を記載しているものもあります。@<br>{}
そういった場合なら、ドキュメントをいちいち確認せずとも、VS Code上で情報をチェックしつつ実装を進めることができます。  


=== @<tt>{request.intent.name}でも試してみよう


もうひとつよく使われるプロパティーがあります。@<br>{}
インテント名が格納される@<tt>{handlerInput.requestEnvelope.request.intent.name}です。@<br>{}
先ほどのコードを、@<tt>{handlerInput.requestEnvelope.request}まで打ち直してから@<tt>{.}を打ってみましょう。@<br>{}
候補の中に@<tt>{intent}オブジェクトは出てきましたでしょうか。おそらく表示されていないでしょう。  



//image[notfound-intent][あるはずの`intent`オブジェクトがない]{
//}




なぜ@<tt>{intent}オブジェクトが候補に現れないのか。@<br>{}
@<tt>{handlerInput.requestEnvelope.request}のコードより、@<tt>{request}の部分を@<tt>{Ctrl (⌘) + クリック}してみましょう。@<br>{}
@<tt>{RequestEnvelope}の型定義が開かれます。@<br>{}
　@<br>{}
続いて@<tt>{request}の型である@<tt>{Request}を@<tt>{Ctrl (⌘) + クリック}してみましょう。@<br>{}
すぐ上の@<tt>{Request}型の定義部にジャンプします。  



//image[request-type][RequestEnvelopeインターフェースとRequest型]{
//}




@<tt>{Request}の定義内容を見てみてください。@<br>{}
@<tt>{events.skillevents.SkillEnabledRequest}をはじめとするいくつもの型が@<tt>{|}で区切られて並んでいます。@<br>{}
@<tt>{|}は型定義においてもOrを表しますので、@<tt>{Request}型はこの大量の型のいずれかになり得るということです。@<br>{}
つまり、Alexaから飛んでくるリクエスト形式もこれだけの種類があるということです。@<br>{}
　@<br>{}
@<tt>{request}オブジェクトにはなぜ@<tt>{intent}オブジェクトが存在しなかったのか。@<br>{}
それは、@<tt>{Request}型がなり得るすべての型に@<tt>{intent}オブジェクトが存在しているわけではないためです。@<br>{}
@<tt>{type}プロパティーは@<tt>{Request}型がなり得るすべての型に存在しているため、@<tt>{request}オブジェクトの候補に出てきたのです。@<br>{}
　@<br>{}
では、@<tt>{intent}オブジェクトは手打ちするしかないのか。もちろんそんなことはありません。@<br>{}
@<tt>{request}オブジェクトの型を絞っていくことで、@<tt>{intent}オブジェクトも候補へ現れるようになります。@<br>{}
逆に@<tt>{intent}オブジェクトが候補に現れないうちは、@<tt>{request}オブジェクトが@<tt>{intent}オブジェクトを持たない型になり得るということであり、手打ちで解決してしまうと予期せぬエラーが発生する可能性があります。  


=== @<tt>{handlerInput.requestEnvelope.request}の型を絞る


型を絞る方法はふたつあります。@<br>{}
ひとつは、型アサーションで明示的に型付けする方法です。@<br>{}
もうひとつは、コードを条件付けしていくことにより型推論を働かせる方法です。  


==== 型アサーションで明示的に型付けする


「@<tt>{変数またはオブジェクト as 型}」という構文で変数またはオブジェクトを明示的にある型へ変換できます。@<br>{}
よって@<tt>{handlerInput.requestEnvelope.request}を、@<tt>{as <intentオブジェクトが存在し得る型>}と型アサーションすればいいわけです。@<br>{}
　@<br>{}
では「<intentオブジェクトが存在し得る型>」とはいったいどんな型なのでしょう。@<br>{}
もう一度@<tt>{request}を@<tt>{Ctrl (⌘) + クリック}し、型定義を開いてみましょう。@<br>{}
@<tt>{Request}型で定義されている型を同様に@<tt>{Ctrl (⌘) + クリック}していくと、いくつか同じファイル内に定義されていることがわかります。@<br>{}
　@<br>{}
ここで「@<tt>{'intent':}」を検索してみましょう。@<br>{}
@<tt>{IntentRequest}インターフェースと@<tt>{CanFulfillIntentRequest}インターフェースが検索に引っかかります。@<br>{}
名前からも、メンバーからも分かるように、@<tt>{IntentRequest}が正解の型となります。  



//image[IntentRequest-type][IntentRequestインターフェース]{
//}




また、開いている型定義のパスを見てみると、@<tt>{node_modules/ask-sdk-model/index.d.ts}となっています。@<br>{}
@<tt>{IntentRequest}は@<tt>{export interface IntentRequest}と記述されているので、この型は@<tt>{ask-sdk-model}をインポートしている変数@<tt>{Model}から読み込めることが分かります。@<br>{}
　@<br>{}
ソースに戻り、次のようにコードを書き換えてみてください。  


//emlist[index.ts（72行目あたり）]{
- handlerInput.requestEnvelope.request
+ ; (handlerInput.requestEnvelope.request as Model.IntentRequest)
//}


この状態で@<tt>{.}を打つと、無事@<tt>{intent}オブジェクトが候補に現れます。  



//image[find-intent1][IntentRequest型アサーションにより`intent`オブジェクトが現れた]{
//}



==== コードを条件付けしていくことにより型推論を働かせる


もうひとつの方法である、型推論も試してみましょう。@<br>{}
「コードを条件付けしていく」とは、ifやtypeof、三項演算などで、型を絞り得る条件を通過した状態にもっていくということです。@<br>{}
　@<br>{}
インテント名である@<tt>{intent.name}が存在するパターンとしては、リクエストタイプである@<tt>{request.type}が@<tt>{IntentRequest}の場合ですね。@<br>{}
これは@<tt>{IntentRequest}の型定義からも読み取れます。@<br>{}
@<tt>{type}プロパティーが@<tt>{'IntentRequest'}と文字列の定数となっています。  


//emlist[IntentRequestの型定義]{
export interface IntentRequest {
    'type': 'IntentRequest';    // `type`が文字列の定数
    'requestId': string;
    'timestamp': string;
    'locale'?: string;
    'dialogState': DialogState;
    'intent': Intent;
}
//}


よって、@<tt>{request.type === "IntentRequest"}と条件付けされた状態であれば、@<tt>{request}の型は@<tt>{IntentRequest}型であるとTypeScriptにより型推論され、@<tt>{request.intent}が候補として現れてきます。@<br>{}
今度は次のようにソースを書き換えてみてください。  


//emlist[index.ts（72行目あたり）]{
- ; (handlerInput.requestEnvelope.request as Model.IntentRequest)
+ handlerInput.requestEnvelope.request.type === "IntentRequest" && handlerInput.requestEnvelope.request
//}


行末の@<tt>{request}のあとに@<tt>{.}を打つと@<tt>{intent}オブジェクトが候補として現れますので、選択してみましょう。@<br>{}
このように条件付けによる型推論によって、暗黙的に型を絞っていくこともできます。  



//image[find-intent2][条件付けでの型推論により`intent`オブジェクトが現れた]{
//}



=== すべての@<tt>{handlerInput}エラーの修正


@<tt>{handlerInput}はハンドラーごとにあり、それらすべてに@<tt>{HandlerInput}型アノテーションを付与する必要があります。@<br>{}
離れた行にある同じ単語を修正するとき、VS Codeでは便利な機能があります。@<br>{}
ここでは、マルチカーソルを使ってすべての@<tt>{handlerInput}を修正してみましょう。@<br>{}
　@<br>{}
次の手順で操作を行います。  

 1. エラーの出ている@<tt>{handlerInput}をダブルクリックし選択
 1. @<tt>{Shift (⇧) + End}で行末尾までの範囲を選択
 1. @<tt>{Ctrl (⌘) + d}で対象をひとつずつ追加選択
 1. すべて選択し終えたら@<tt>{handlerInput}の後ろに@<tt>{: Alexa.HandlerInput}と入力
 1. 最後に@<tt>{Esc}でマルチカーソルを解除



//note[さまざまなマルチカーソルのショートカット]{
マルチカーソルには、@<tt>{Ctrl (⌘) + d}でひとつずつ選択していく以外にも、次のショートカットがあります。@<br>{}
　  

 * @<tt>{Ctrl (⌘) + Shift (⇧) + l}：選択範囲と同じワードをすべて選択
 * @<tt>{Shift (⇧) + Alt + i}：選択範囲の各行の行末にマルチカーソルを挿入
 * @<tt>{Ctrl (⌘) + Alt + ↑ or ↓}：現在のカーソル位置の上下にマルチカーソルを挿入
 * @<tt>{Alt + クリック}：任意の場所にマルチカーソルを挿入



　@<br>{}
マルチカーソルはVS Codeのとても強力な機能のひとつですので、ぜひとも各ショートカットを手に馴染ませておくことをオススメします。
//}



//clearpage


=== @<tt>{throwResult}の型


エラーも最後のひとつまで絞られました。@<br>{}
@<tt>{throwResult}の型を探してみましょう。@<br>{}
　@<br>{}
エラーが発生しているのは@<tt>{resultText}関数の引数部です。@<br>{}
ひとまず@<tt>{resultText}関数がどこで使われているかを探すと、@<tt>{getThrowResultMessage}関数内（27行目）で使われています。@<br>{}
ここで引数に@<tt>{throwResult}が指定されていますが、そこにマウスオーバーしてみると次の図のように型が表示されます。@<br>{}
結論からいうと、この@<tt>{{ midText: string; sum: number; diceCount: number; \}}というオブジェクト型が正解です。  



//image[throwresult][throwResultの型]{
//}




しかし、この@<tt>{{ midText: string; sum: number; diceCount: number; \}}というオブジェクト型はソース上のどこにも記述されていません。@<br>{}
ではどこから生まれたのかというと、@<tt>{throwDice}関数の戻り値から型推論されたのです。@<br>{}
このように変数の型だけでなく、関数の戻り値でも型推論は行われます。  


=== 型を切り出して宣言


正解の型は@<tt>{{ midText: string; sum: number; diceCount: number; \}}というオブジェクト型ですので、このまま型アノテーションをつけてもいいのですが、少し記述が冗長です。@<br>{}
今回は1ヵ所のみへの指定のためあまり影響はありませんが、これが複数ヵ所に渡ると可読性を損ねます。@<br>{}
そのため、オブジェクト型など型の記述が長くなる場合には、型を切り出して宣言しましょう。@<br>{}
　@<br>{}
次のように、@<tt>{type}から始まる記述で型を宣言できます。@<br>{}
ここでは@<tt>{ThrowResult}という名前で型を宣言しています。@<br>{}
型名にはパスカルケース（先頭大文字）を用います。  



//clearpage


//emlist[index.ts（18行目）]{
+ type ThrowResult = { midText: string; sum: number; diceCount: number; }
//}

=== @<tt>{ThrowResult}型アノテーションをつける


エラーが発生している@<tt>{resultText}関数の引数@<tt>{throwResult}に、@<tt>{ThrowResult}型アノテーションを付与します。  


//emlist[index.ts（56行目）]{
- const resultText = (throwResult) => {
+ const resultText = (throwResult: ThrowResult) => {
//}


@<tt>{throwDice}関数の戻り値にも、@<tt>{ThrowResult}型アノテーションを付与してみましょう。@<br>{}
戻り値に型アノテーションをつける場合は、引数の@<tt>{()}のあとに@<tt>{: type}のように記述します。  


//emlist[index.ts（34行目）]{
- const throwDice = (diceCount: number) => {
+ const throwDice = (diceCount: number): ThrowResult => {
//}


このようにすることで、誤って戻り値の構造を変えてしまったときでもエラーで気付くことができます。@<br>{}
試しにreturnしているオブジェクトから@<tt>{diceCount}を削ってみましょう。  


//emlist[index.ts（50行目）]{
- return { midText, sum, diceCount }
+ return { midText, sum }
//}


すると次の図のエラーが発生します。  



//image[throwresult-error][ThrowResult型情報不足エラー]{
//}



//emlist[ThrowResult型情報不足エラー]{
Property 'diceCount' is missing in type '{ midText: string; sum: number; }' but required in type 'ThrowResult'.ts(2741)
index.ts(18, 52): 'diceCount' is declared here.
//}


戻り値の@<tt>{ThrowResult}型に対して、returnしているオブジェクトに@<tt>{diceCount}が不足しているというエラーです。@<br>{}
戻り値の型も明示的に指定することで、このようにTypeScriptのエラーですぐにバグに気付けます。@<br>{}
　@<br>{}
エラーを確認したらソースは元に戻しておきましょう。  


//emlist[index.ts（50行目）]{
- return { midText, sum }
+ return { midText, sum, diceCount }
//}

=== @<tt>{interface}での型宣言


@<tt>{type}での型宣言以外にも、@<tt>{interface}を使った型宣言もできます。  


//emlist[index.ts（18行目）]{
- type ThrowResult = { midText: string; sum: number; diceCount: number; }
+ interface ThrowResult { midText: string; sum: number; diceCount: number; }
//}


宣言方法により@<href>{https://qiita.com/tkrkt/items/d01b96363e58a7df830e,型自体の表現や拡張性などに違いがあります}@<fn>{url6}。@<br>{}
基本的にどちらを使えばいいのかというと、@<tt>{type}の方が多機能ですのでこちらをおすすめします。@<br>{}
ただし@<tt>{interface}では継承など、@<tt>{type}では実現できない機能もありますので、必要に応じて@<tt>{interface}の使用も検討してみてください。  



//footnote[url6][参考：@<href>{https://qiita.com/tkrkt/items/d01b96363e58a7df830e,TypeScriptのInterfaceとTypeの比較（Qiita）}@<br>{}（@<href>{https://qiita.com/tkrkt/items/d01b96363e58a7df830e}）]


=== コンパイル & Node実行 & テスト


これですべてのエラーを潰すことができました。@<br>{}
次のコマンドでコンパイルとNode実行を行い、ブラウザよりテストを行ってみましょう。@<br>{}
JavaScriptのときと同じ動きになっていれば、TypeScriptへの移植成功です。  


//cmd[]{
npx tsc
node dist/index.js
//}


//clearpage


== 関連ツールについて


この節ではTypeScriptでスキル開発を行ううえで、さらに超効率爆速開発できるツールとその使い方を紹介します。  


=== ESLint


ESLintとは、指定のルールでコードを記述しているかどうかをチェックするツールです。@<br>{}
たとえば、インデントはタブかスペースか（スペースなら2つか4つか）、文字列は必ずダブルクォーテーションでくくっているか、といったルールを指定しチェックできます。@<br>{}
VS Code拡張機能と合わせて使うことで、リアルタイムにコードをチェックできます。  


==== @<tt>{typescript-eslint}のインストール


次のコマンドでインストールを行います。@<br>{}
ESLint本体に加え、TypeScript用プラグインとパーサーも一緒にインストールします。@<br>{}
@<tt>{@typescript-eslint/eslint-plugin}はESLintへTypeScriptの固有ルールを追加し、@<tt>{@typescript-eslint/parser}はESLintをTypeScriptの構文に対応させます。  


//cmd[]{
npm install -D eslint @typescript-eslint/eslint-plugin @typescript-eslint/parser
//}


VS CodeにもESLintの拡張機能をインストールします。@<br>{}
拡張機能パネルにて@<tt>{eslint}で検索し、有効化します。  



//image[vscode-eslint][VS Code拡張機能のESLint]{
//}



==== ESLintの初期化


次のコマンドを実行し初期化を行い、@<tt>{.eslintrc.json}を生成します。@<br>{}
対話形式コマンドですので、次のコマンドログを参考に選択していってください。@<br>{}
ハイライト部が選択項目です。@<br>{}
@<tt>{Where does your code run?}はチェックボックス形式ですので、@<tt>{Browser}のチェックを外し@<tt>{Node}のみを選択してください（スペースキーでオン／オフできます）。  


//cmd[]{
$ npx eslint --init

? How would you like to use ESLint? @<cursor>{To check syntax, find problems, and enforce code style}
? What type of modules does your project use? @<cursor>{JavaScript modules (import/export)}
? Which framework does your project use? @<cursor>{None of these}
? Does your project use TypeScript? @<cursor>{Yes}
? Where does your code run? @<cursor>{Node}
? How would you like to define a style for your project? @<cursor>{Answer questions about your style}
? What format do you want your config file to be in? @<cursor>{JSON}
? What style of indentation do you use? @<cursor>{Tabs}
? What quotes do you use for strings? @<cursor>{Double}
? What line endings do you use? @<cursor>{Unix}
? Do you require semicolons? @<cursor>{Yes}
//}


以上で@<tt>{.eslintrc.json}が生成されましたので、@<tt>{index.ts}と問題パネルを開いてみてください。@<br>{}
主に行末のセミコロンの有無について、多数のESLintエラーが発生していることでしょう。  



//image[linterror][ESLintのエラー]{
//}



==== TypeScript用リントルールの指定


TypeScript用のリントルールを指定するときは、@<tt>{rules}配下に@<tt>{@typescript-eslint/...}と追記します。@<br>{}
このときの注意点として、本家ESlintと一部重複するルールがあり@<fn>{annotation2}、それらを使う場合は本家ESLint側のルールを無効化する必要があります。  


//emlist[.eslintrc.jsonルール追記例]{
        "rules": {
            "no-unused-vars": "off", // 本家ESLint側では無効化
            "@typescript-eslint/no-unused-vars": "error"
        }
//}


リントルールについては、次のサイトをご覧ください。  



@<href>{https://github.com/typescript-eslint/typescript-eslint/tree/master/packages/eslint-plugin#supported-rules,@typescript-eslintルール一覧}@<fn>{url7}



//footnote[annotation2][本家ESLintのままではTypeScriptのコードで誤検知があるルールについて、TypeScript対応したものを@<tt>{typescript-eslint}で提供している]
//footnote[url7][@<href>{https://github.com/typescript-eslint/typescript-eslint/tree/master/packages/eslint-plugin#supported-rules}]


=== Prettier


Prettierとは、ソースコードを整形するためのコードフォーマッターです。@<br>{}
さまざまな言語、エディターに対応しており、JavaScript/TypeScriptのコードフォーマッターではデファクトスタンダードです。@<br>{}
ESLintと合わせて使用するケースがほとんどですが、一部機能が重複しているため、組み合わせて使うには少し設定が必要です。  


==== Prettierのインストール


次のコマンドでインストールを行います。@<br>{}
Prettier本体に加え、ESLint用プラグインも一緒にインストールします。@<br>{}
@<tt>{eslint-plugin-prettier}はESLintと連携するために必要で、@<tt>{eslint-config-prettier}はESLintと重複するチェックを無効化します。  


//cmd[]{
npm install -D prettier eslint-plugin-prettier eslint-config-prettier
//}


VS CodeにもPrettierの拡張機能をインストールします。@<br>{}
拡張機能パネルにて@<tt>{prettier}で検索し、有効化します。  



//image[vscode-prettier][VS Code拡張機能のPrettier]{
//}




//clearpage


==== @<tt>{.eslintrc.json}の編集


PrettierをESLintと連携するため、次のように@<tt>{.eslintrc.json}を編集します。  


//emlist[.eslintrc.json（6行目あたり）]{
    "extends": [
        "eslint:recommended",
-       "plugin:@typescript-eslint/eslint-recommended"
+       "plugin:@typescript-eslint/eslint-recommended",
+       "plugin:prettier/recommended",
+       "prettier/@typescript-eslint"
    ],
//}


この状態で@<tt>{index.ts}と問題パネルを開いてみてください。@<br>{}
@<tt>{.eslintrc.json}を生成したときは、主にセミコロンの有無に関するエラーが出ていましたが、さらにインデントに関するエラーが発生しています。  



//image[prettiererror][Prettier追加後のエラー]{
//}



==== Prettierによるコード整形


続いて、Prettierによるコード整形を試してみましょう。@<br>{}
コード整形のショートカットである@<tt>{Shift (⇧) + Alt + f}を押してみてください。@<br>{}
このとき、既定のフォーマッターの選択をVS Codeより求められた場合は、Prittierを選択してください。@<br>{}
コード整形により、セミコロンの追加と、インデントがタブからスペース2つに変わりました。  


==== ESLintとのルール差


ここで再び@<tt>{.eslintrc.json}を開いてみてください。@<br>{}
@<tt>{rules}に次の記述があります。  


//emlist[.eslintrc.json（24行目）]{
    "rules": {
        "indent": [
            "error",
            "tab"
        ],
//}


この記述は、@<tt>{.eslintrc.json}の生成時の対話コマンドラインにて自動的に記述されたルールで、インデントをタブのみ許容しています。@<br>{}
にもかかわらず、Prettierによってスペース2つに整形されてしまいました。@<br>{}
　@<br>{}
これは先ほどインストールした@<tt>{eslint-config-prettier}による効果で、ESLintと重複したルールについて、ESLintのルールを無視し、Prettierのルールを優先します。@<br>{}
そのため、重複するルールについてはPrettier側のルールを記述しましょう。  


==== Prettierのルール指定


Prettierのルールも@<tt>{.eslintrc.json}に記述します。@<br>{}
次のようにESLintのルールをごっそり削除し、Prettierのルールを追記してください。  


//emlist[.eslintrc.json（24行目あたり）]{
    "rules": {
-       "indent": [
-           "error",
-           "tab"
-       ],
-       "linebreak-style": [
-           "error",
-           "unix"
-       ],
-       "quotes": [
-           "error",
-           "double"
-       ],
-       "semi": [
-           "error",
-           "always"
-       ]
+       "prettier/prettier": ["error", {
+           "parser": "typescript",
+           "useTabs": true,
+           "endOfLine": "lf"
+       }]
//}


@<tt>{"quotes"}（ダブルクォーテーション）、@<tt>{"semi"}（セミコロンあり）のルールが消えたように見えますが、これらのルールはPrettierではデフォルト値のため記述する必要がありません。@<br>{}
Prettierのルールの一覧、デフォルト値については公式ドキュメントをご覧ください。  



@<href>{https://prettier.io/docs/en/options.html,Prettier Options}@<fn>{url8}



//footnote[url8][@<href>{https://prettier.io/docs/en/options.html}]


==== ファイル保存時に自動でコード整形する


コードを書くたびに、いちいち手動でコード整形を行うことは面倒なので、VS Codeでファイルを保存する際に自動でコード整形を行うようにしてみましょう。@<br>{}
次の方法でVS Codeの設定ファイルである@<tt>{settings.json}を開きます。  

 1. @<tt>{Ctrl (⌘) + Shift (⇧) + p}を押してコマンドパレットを開く
 1. @<tt>{setting json}と入力し、絞られる候補の中から「@<tt>{>Preferences: Open Settings (JSON)}」を選択（「@<tt>{>Preferences: Open Default Settings (JSON)}」ではありません！）



@<tt>{settings.json}が開かれたら、次のコードを追記します。  


//emlist[settings.json]{
+   "editor.codeActionsOnSave": {
+       "source.fixAll.eslint": true
+   },
//}


この状態で再度@<tt>{index.ts}をファイルを開き、ファイル保存をしてみましょう。@<br>{}
保存時に自動でコードが整形されます。  


=== tsc -w


TypeScriptのコンパイルを行う@<tt>{tsc}コマンドですが、@<tt>{-w}オプションを付け加えるとウォッチモードで実行できます。@<br>{}
ウォッチモードでは、ソースを監視し変更があれば自動でコンパイルを行います。@<br>{}
@<tt>{tsc}コマンド単体で実行するとコンパイルに若干時間がかかりますが、@<tt>{-w}オプションを付け加えれば起動時以降は高速にコンパイルされます。  


//cmd[]{
npx tsc -w
//}

=== @<tt>{nodemon}


@<tt>{nodemon}とは、ソースの変更を検知して、プロセスを自動で再起動してくれるツールです。@<br>{}
これを導入することで、ソースを修正するたびにいちいちプロセスを手動で再起動する、といった手間が不要になります。@<br>{}
　@<br>{}
次のコマンドでインストール、実行を行います。@<br>{}
一度実行しておけば、ソースが変更される度に自動で再起動してくれます。@<br>{}
また、@<tt>{-w}オプションにより特定のディレクトリのみを指定して監視できます。  


//cmd[]{
npm install -D nodemon
npx nodemon -w dist dist/index.js
//}

=== 開発Tips


Alexaスキルをローカル開発するときは、@<tt>{ngrok http 3000}、@<tt>{tsc -w}、@<tt>{nodemon -w dist dist/index.js}を同時に動かしておくことをおすすめします。@<br>{}
そうすることで、ソース変更 -> 自動でコンパイル -> 自動でnode再起動、となり、幸せが訪れます。  



私は普段Windowsで開発しており、次のような一発起動バッチを使っています。  


//emlist[develop.cmd]{
@echo off
start npx ngrok http 3000
start npx tsc -w
start npx nodemon -w dist dist/index.js
//}

=== VS Codeでのデバッグ


VS Codeでのデバッグ方法について説明します。@<br>{}
@<tt>{ngrok}も連携し、ソース修正時にデバッガーを再起動することなくデバッグを行います。  


==== TypeScriptの@<tt>{Source Map}有効化


コンパイルされたJavaScriptソースの行が、TypeScriptソースのどの行に相当するかをデバッガーへ伝えるために、@<tt>{Source Map}ファイルが必要になります。@<br>{}
@<tt>{Source Map}を生成するよう@<tt>{tsconfig.json}を修正します。  


//emlist[tsconfig.json（13行目あたり）]{
- // "sourceMap": true,
+ "sourceMap": true,
//}


コンパイルを行い、@<tt>{index.js.map}が生成されることを確認します。  


//cmd[]{
$ npx tsc
$ ls dist/index.js.map
dist/index.js.map
//}

==== デバッグ設定の初期化


VS Codeにて、デバッグ設定の初期化を行います。@<br>{}
デバッグパネルを開き（@<tt>{Ctrl (⌘) + Shift (⇧) + d}）、次の図にある「デバッグと実行をさらに構成するにはlaunch.jsonファイルを作成します。」のリンクをクリックします。  



//image[debug-init][デバッグパネル]{
//}




これにより、@<tt>{.vscode/launch.json}が作成され、VS Code上に開かれます。  


==== @<tt>{nodemon}対応


@<tt>{nodemon}に対応するよう、次のように@<tt>{launch.json}を書き換えます。  


//emlist[launch.json（6行目あたり）]{
    "configurations": [
        {
            "type": "node",
-           "request": "launch",
+           "request": "attach",
            "name": "プログラムの起動",
            "skipFiles": [
                "<node_internals>/**"
            ],
-           "program": "${workspaceFolder}\\src\\index.ts",
+           "processId": "${command:PickProcess}",
+           "restart": true,
+           "protocol": "inspector",
            "outFiles": [
                "${workspaceFolder}/**/*.js"
            ]
        }
    ]
//}

==== ブレークポイントを置く


ブレークポイントとは、デバッグ実行時に処理がブレークポイントを置いた行を通るとき、処理を一時停止できる機能です。@<br>{}
一時停止した時点の変数の状態などを確認できるので、@<tt>{console.log}をハードコーディングするようなデバッグに比べ効率的です。@<br>{}
また、条件付きブレークポイントやログポイントも置くことができます。@<br>{}
　@<br>{}
試しに、@<tt>{launchHandler}の@<tt>{handle}メソッド内の最初の行（79行目）にブレークポイントを置いてみましょう。@<br>{}
VS Codeにてソースパネルの行数の左側をクリックします。@<br>{}
次の図のように赤丸がついたらブレークポイントを置いた状態となります。  



//image[breakpoint][ブレークポイント]{
//}



==== デバッグの実行


@<tt>{nodemon}を@<tt>{--inspect}オプションをつけて実行します。  


//cmd[]{
npx nodemon --inspect -w dist dist/index.js
//}


次にVS CodeでF5キーを押し、デバッグを実行します。@<br>{}
デバッグプロセスを聞かれるので、@<tt>{node inspect}と入力し絞られたプロセスを選択してください。  



//image[debug-attach][デバッグプロセスのアタッチ]{
//}



==== スキルの実行


ブラウザの@<tt>{テスト画面}にてスキルを起動します。@<br>{}
するとVS Codeにてブレークポイントの位置で処理が停止し、そのときの変数状態がチェックできます。@<br>{}
　@<br>{}
この状態からF5キーを押すと以降の処理が進みます。@<br>{}
F10キーを押すと現在の行の処理を行い、次の行で停止します。  



//image[debug-break][デバッグ実行時にブレークポイントで停止している様子]{
//}



==== タイムアウトへの対処法


@<tt>{テスト画面}の@<tt>{Alexaシミュレータ}や実機からスキルを起動した場合、ブレークポイントでの一時停止によりレスポンスがしばらく返ってこないと、タイムアウトしてしまいます。@<br>{}
タイムアウトしてしまうと最終的なAlexaの発話や、返ってくるJSONがチェックできません。@<br>{}
　@<br>{}
そういった場合は、@<tt>{テスト画面}の右側に表示される@<tt>{スキルの呼び出し}より@<tt>{JSON入力}に表示されるJSONを@<tt>{ngrok}エンドポイントへ直接POSTしてみましょう。  



//image[input-json][スキルの呼び出し JSON入力]{
//}




さくっとPOSTする方法としては、VS Code拡張機能の@<tt>{REST Client}をオススメします。@<br>{}
拡張機能パネルにて@<tt>{rest}で検索し、有効化してみましょう。  



//image[vscode-rest][VSCode拡張機能のREST Client]{
//}




使い方としては、まずVS Codeにて@<tt>{Ctrl (⌘) + n}で新規ファイルを作成し、次のように記述します。@<br>{}
@<tt>{https://xxxxxxxx.ngrok.io}の部分は、スキルのエンドポイントに指定した@<tt>{ngrok}のURLと同じURLを記述します。@<br>{}
4行目以降のJSONは、Alexa開発者コンソールの@<tt>{テスト画面}に表示される@<tt>{JSON入力}内のJSONを入力します。  


//emlist[新規ファイル]{
POST https://xxxxxxxx.ngrok.io
content-type: application/json

{
    "version": "1.0",
    "session": {
        "new": true,
        ...
}
//}


そして@<tt>{Ctrl (⌘) + Alt + r}を押すだけで、POSTリクエストを@<tt>{ngrok}エンドポイントへ送ってくれます。@<br>{}
これならデバッグ中でもタイムアウトは発生せず、最終的なAlexaの発話や、返ってくるJSONもVS Code上で確認できます。  



//image[post][REST Client拡張機能によるPOSTリクエスト]{
//}



== おわりに


簡単にでしたが、以上がTypeScriptでのAlexaスキル開発方法になります。@<br>{}
TypeScriptは、その名にも冠するように型のしくみがとても重要です。@<br>{}
「型パズル」というワードが生まれるぐらいに型の沼は深いので、ぜひ次のステップでは型について深く学んでみてください。@<br>{}
　@<br>{}
また、Alexaは2019年に「Alexa Web API for Games」や「Alexa Gadgets Toolkit」といった新たなAPIをどんどん公開しています。@<br>{}
新たな仕様に沿ったコードを書くときにも、TypeScriptの型による入力補完や、バグの予防はとても有効ですので、ぜひTypeScriptによる超効率爆速開発でまわりに差をつけてください。  


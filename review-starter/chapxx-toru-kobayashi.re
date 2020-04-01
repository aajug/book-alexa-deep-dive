= VUIクリエイターのためのコンテンツ作成ガイド

//image[tag][]{
//}

//flushright{
著者：ボイスアップラボ コバヤシトール（@<href>{https://twitter.com/voiceapplab/,@voiceapplab}）
//}

== はじめに 
スマートスピーカーが日本にやってきたのが2017年冬。招待制の Amazon Echo にやきもきしながら、スキル作りに夢を膨らませていたのを思い出します。あれから2年、スマートスピーカーを取り巻く環境はずいぶん変わりましたよね。一般の家庭でも見かけることが多くなり、 Alexa のスキルも日本だけで3000個、 US では10万個を超え、その勢いは増すばかりです。

//blankline
でもそのかわり、スキルをきちんとユーザーさんに届けることがどんどん難しくなってきていると思いませんか？皆さんのスキルはどうですか？ちゃんとユーザーさんに使ってもらってますか？

//indepimage[smartspeaker][][scale=0.7]{
//}

どもどもはじめまして。ボイスアップラボ株式会社 CEO のコバヤシトールといいます。ボイスアップラボは、AlexaやGoogle Home、LINE Clovaなどのスマートスピーカーのスキルを研究開発するために、 2017年の秋、三名のフリーランスエンジニアによるユニットとしてスタートし、その後2年あまりで50本以上のスキルをリリースしてきました。

//blankline
僕自身もエンジニアとして、特にエンターテイメント系の作品を中心に多数のスキルを作っています。おかげさまで LINE BOOT AWARDS や Alexa Developer スキルアワード など、オフィシャルのアワードで何度か賞をいただきました。@<fn>{awords} 特に2019年は Alexa スキルアワードでグランプリ（「どうぶつびより」で受賞）をいただくという嬉しい出来事もありました。
//footnote[awords][「ゾンビのまち」で LINE BOOT AWARDS 2018 部門賞を受賞。「キャプテン九九」で Alexa Developer スキルアワード 2018 ファイナリストに選出。]

//blankline
僕らのような無名のエンジニアが戦うには、音声スキル市場は楽しくもあり、また過酷でもあります。有名な自社サービスやIP@<fn>{ip} を持たない弱小エンジニアが、どのようにマーケットに挑めば勝つことができるのか、そればかりを考えていました。
//footnote[ip][Intellectual Property = 知的財産とは、発明やデザイン、著作物など、人間が創造的活動により生み出したものを指します。 ]

//blankline
そのなかで一番重要に感じたのは「コンテンツ力」です。コンテンツと聞くと「いやー、俺はIP持ってないからなぁ。使えるコンテンツがあればなぁ。」とため息をつくエンジニアの方が結構いらっしゃいますが、いえいえ、そんなことはないのです。コンテンツがなければ作れば良いのです。実は「コンテンツなんて誰でも作れる」のです。

//blankline
この章はまさにそんな方向けの章です。技術書なのにコードは一切出てきません。スキルを作ったことがない人、はたまたエンジニアじゃない人もこの章の対象となります。この章を読み追えたときに@<b>{「VUIコンテンツ、俺もやってみようかな」}と少しでも思ってもらえたら幸いです。

===== 本章の対象読者@<br>{}
本章の対象読者は、次のような方です。

 * スキル開発に興味があるエンジニア、デザイナー、ディレクターさん
 * ユーザーに使ってもらえるようなスキルを作りたい方
 * 「コンテンツ」と聞くとついつい身構えてしまうエンジニアさん
 

== コンテンツとは何？
=== コンテンツの正体
//indepimage[contents][][scale=0.7]{
//}

はじめに、コンテンツっていったい何でしょう？辞書で調べてみると次のように書いてあります。

//source{
@<b>{コンテンツ}：① 中身。② 情報の内容。放送やネットワークで提供される動画・音声・テキストなどの情報の内容をいう。（三省堂 大辞林）
//}

ふむふむ、たとえば漫画やゲーム、音楽、テレビ番組などは、放送やネットワークで提供されるコンテンツとしてイメージできそうですね。しかし「コンテンツ」という言葉に内包される意味はそれだけでしょうか？そもそも、コンテンツはどうやって生まれるのでしょうか？ちょっと視点を変えて、コンテンツであるものとコンテンツでないものの境界線を考えてみましょう。

//blankline
たとえば、次のキーワードはコンテンツでしょうか？ 

//source{
A. コーンフレーク
//}

コーンフレーク？なんのこっちゃ、 と思いましたか？ではこれではどうでしょう？

//source{
B. コーンフレーク違うやないかい
//}

ピンとくる人はきっとお笑いマニアですね。そう、2019年 M-1 グランプリ@<fn>{m1} で優勝した「ミルクボーイ」のネタです。おかんの大好きな朝ごはんの名前を一生懸命思い出すというストーリーで、M-1史上最高得点（681点）を叩き出した歴史的なネタです。
//footnote[m1][吉本興業が主催する若手漫才師による漫才のコンクール。2001年から始まり、2010年で終了したが、2015年から復活し、毎年12月に開催されている。]

//blankline
この二つのワード、似ているようで内包されているイメージが全く違います。
前者はただのコーンフレークという言葉です。なんのイメージも持ち合わせません。しかし後者のイメージは特別です。M-1 グランプリを見た人、やミルクボーイファンにとっては聞くだけで笑ってしまったり、「そうそう、オカンがな…」と乗りたくなる話題でしょう。

//indepimage[manzai][][scale=0.7]{
//}


これが「コンテンツ」の原型であると僕は考えています。僕なりにコンテンツを定義しますと、

//memo{
//noindent
@<b>{コンテンツとは、そのことについて意味を見出し楽しむ人々（＝オーディエンス）がいる事柄}
//}

//noindent
となります。さあ、ついてきてくださいね。

//clearpage
=== 誰でもコンテンツを作ったことがある

なるほど、コンテンツの定義はわかったけど、そんなものを作れるのは一部のクリエイターだけじゃないの、と思ってませんか？
そうではありません。実はあなたもコンテンツを作ったことがあります。

//blankline
たとえば居酒屋で、同僚と飲んでる時に、なにか面白いネタを言ったとします。 「部長って、アルパカに似てるよね！」とかにしましょう。すると同僚たちは「言えてる〜」「わかるわ〜」などと言いながら大笑いしました。次の日も部長の訓示を聴きながらクスクス笑っています。飲み会でありそうなノリですよね。

//indepimage[alpaka][][scale=0.7]{
//}


実はこれ、立派なコンテンツです。 この場合@<b>{「オーディエンス＝同僚」}です。あなたは同僚にだけわかる面白いネタを提供したので、彼らは「部長がアルパカに見える」という意味を楽しむことができました。つまりあなたはコンテンツを提供し、彼らは消費したわけです。

//blankline
このように、あなたは知らず知らずのうちにコンテンツを生み出し、オーディエンスによって受け入れられた経験をしています。いわばクリエイティブの原体験ですね。この感覚、とっても大切ですので、いつでも思い出せるようにしておきましょう。

=== オーディエンスをどこに設定するか
さて、上記のアルパカのネタとミルクボーイ が M-1で披露したコーンフレークのネタ、テキストだけを比べてみれば、その内容にそれほどの差はありません。
ただ一つ大きな違いは「オーディエンスをどこに設定するか」ということです。
アルパカネタのオーディエンスは「同僚」に、コーンフレークネタは「一般人」向けに設定されているといって良いでしょう。

//source{
・アルパカネタのオーディエンス　@<b>{→同僚（職場、 コミュニティ）}
・コーンフレークネタのオーディエンス　@<b>{→一般人（TV視聴者、マス）}
//}

コミュニティ内であんなに面白かったネタなのに、外部の人に話すと全くウケない事ってありますよね？いわゆる「内輪ウケ」が発生してしまう原因は「異なるオーディエンスにコンテンツをぶつけてしまう」ことにあります。でもそれを逆に考えると「オーディエンスがわかれば、コンテンツの作り方もわかる」と言えそうです。ではそもそもオーディエンスって何なのでしょう？

=== オーディエンスは「共感ポイント」で形作られる
ここでオーディエンスの定義を僕なりに言いますと、

//memo{
//noindent
@<b>{オーディエンスとは「共感ポイント」を共有するグループのこと}
//}

//noindent
です。たとえば「同僚」というオーディエンスにおける共感ポイントは「部長」です。部長の風貌や人柄を知っているグループがそのネタのオーディエンスとして成立するわけです。

//blankline
とすると、コーンフレークネタの共感ポイントはどこでしょう？それは当然コーンフレークが持つ「パブリックイメージ」でしょう。ほとんどの人は、コーンフレークを食べたことがあるはずです。コーンフレークに対する「朝食べる、手軽、栄養たっぷり」というイメージをてこにして、それをズラしたり、強調したりすることでコンテンツ化しているのです。

//blankline
部長のイメージを、全国の人と共有できませんが、コーンフレークならできますよね？このように、オーディエンスを近くの同僚や家族に持ってくるか、または遠くの一般人やマスに持ってくるかで共感ポイントは変わってきます。 逆に言えば@<b>{「共感ポイントの置き方次第でオーディエンスの範囲が決定される」}と言えるのです。

//image[audience][オーディエンスの範囲][scale=0.7]{
//}



共感ポイントを見つける作業は、@<b>{「コンテンツづくりの核」}となります。共感ポイントをどこにするか、という判断がコンテンツの成否を分けると言ってもよいでしょう。 
コンテンツの概論はこれぐらいにして、次からはいよいよVUIにおけるコンテンツづくりの話に入っていきます。

//clearpage
== VUI的 共感ポイントの見つけ方
ここまでで、次のことがわかりました。

//memo{
//noindent
@<b>{1. コンテンツとは、そのことについて意味を見出し楽しむ人々（＝オーディエンス）がいる事柄}

@<b>{2. オーディエンスとは「共感ポイント」を共有するグループのこと}
//}

さあ、これをVUIに応用してみましょう。オーディエンスは@<b>{「ターゲットユーザー」}と置き換えることができます。スキルを届けたいユーザー層をまず思い浮かべましょう。小学生の子どもでも良いでしょうし、または20代のサラリーマンや主婦、あるいはシニア層でも良いでしょう、特定のターゲットユーザーにフォーカスを当ててみます。

//blankline
ターゲットが決まったらイメージを膨らませます。彼らの「共感ポイント」はどこでしょうか？彼らが思わず「わかるわかる！」「それそれ！」と感じるような事柄があれば、それが共感ポイントです。楽しいこともあれば、お困りごともあるかもしれませんね。UXデザインのマナーに従えば、ここでペルソナを作ってみるのも良いでしょう。彼らの生活をできるだけリアルにイメージしてみます。

//indepimage[persona][][scale=0.7]{
//}


・・・おっと、あなたの心の声が聞こえてきました。
「いやいや、ペルソナ作ってユーザーをイメージしましょうっていつも言うけど、あれ結構ムズいやん。ピントがずれるとトンチンカンな方向に行くし。もっと簡単な方法ってないの？」

//blankline
あります。実はVUI、共感ポイントを見つけるもっと楽な方法があるんです。

=== VUIは最初からサウンドにフォーカスするのが近道
一番の近道は、共感ポイントを@<b>{「サウンドにフォーカス」}するやりかたです。VUIでのコア体験は、スマホのアプリやWebサービスと異なり、

//clearpage

//source{
@<b>{・サウンド体験}
@<b>{・会話体験}
//}

に絞られます。最近はディスプレイ付きのデバイスも増えてきましたが、たとえグラフィックがあったとしてもそれは補助的で、ユーザーは常に音声に耳をかたむけ、声でデバイスと対話することに変わりありません。 

//blankline
一文を聞くのにも時間がかかるというVUIの特性上、多くのユーザーは、スキルが発するサウンドや会話を注意深く聞き、好きか嫌いか、納得感があるかないかを瞬時にジャッジします。そうですね、だいたい2ターンぐらいで「あ、これ好き」「これ嫌い」と判断するのではないでしょうか。たとえばゲームなのに長いルール説明ではじまるスキルや、サウンド系なのに音質や音量が適切でないスキル、生活習慣系なのに設定項目が多いスキルなどに遭遇すると、がっかりして二回目を起動する気になりませんよね？

//blankline
わかっているはずなのに、そういうスキル、ついつい作ってしまいます。なぜ作ってしまうかというと「共感づくり」を後回しにしてしまっているからです。自分の作りたい仕組みを優先したり、音声と無関係な体験からニーズを探るような作り方をすると、VUIに落とし込んだときに、強引な展開になったり、意味がわからないスキルになりがちです。 共感ポイントを見つけられないと、ユーザーは「疎外感」を感じてしまいます。内輪ネタと同じように、ユーザーに「苛立ちやつまらなさ」を感じさせてしまうのです。

//blankline
それを防ぐにはどうすればよいか、もうわかりますね？
そうです、ターゲットユーザーの「音声体験」に寄り添って考えてみるのです。「サウンドや会話」に絞って共感ポイントを探してみるのです。
音声体験にフォーカスした共感ポイントが見つかれば、それをてこにすることで、初回起動時におっと気を引く転換にできたり、ユーザーに親しみをもってもらえるコンテンツになります。
いわば@<b>{「共感サウンド」}ですね。これ一緒に探ってみましょう。

=== サウンドや会話に着目して共感サウンドを探す
==== 「サウンドバス」で共感サウンドを引き寄せる
@<b>{「カラーバス」}という言葉をご存知でしょうか？心理学の用語で「特定の色を意識すると、知らないうちにその色への注目が高まる」という人間の心理作用を意味しています。アイデアを引き寄せる手法として、発想法でよく用いられます。
たとえば朝起きて会社に行くまでの間に「黄色いものに注目する」と心に決めます。すると不思議なことに、黄色いものばかりが目に入ってくるようになります。たとえば、

//source{
道路に書かれたセンターライン、 工事中や踏切の標識、 黄色い軽自動車、小学生のランドセル、土建屋さんのヘルメット、車のウインカー。
//}

//noindent
などなど。これらを逐次メモするだけで、ある黄色をキーファクターとしたアイディアを集めることができるというわけです。

//indepimage[car][][scale=0.7]{
//}


この方法を応用した@<b>{「サウンドバス」}という手法をやってみましょう。ちなみにサウンドバスは僕が勝手に作った造語です。

//blankline
朝起きて会社に行くまで、または仕事中や家に帰ってくるまででも良いでしょう。ひたすらサウンドに注目してみてください。誰かとの会話でもいいですね。気になったことがあったらメモをしたり、録音したりして集めてみるのです。
こうすると不思議なことに、街の中はいろんな音に溢れていることに気づきます。実際に1日かけてやってみることをおススメしますが、ターゲットとなるオーディエンスのことを思い浮かべてイメージしてみるのも良いでしょう。

==== 小学４年生のA君の場合
試しに、ターゲットユーザーを「小学４年生のA君」にして、サウンドバスしてみましょう。彼は日常生活でどんな「サウンド」に接しているでしょうか？イメージしてみます。

//blankline
まず朝起きます。@<b>{「ジリリリリリ！」}おっと、目覚ましが鳴りました。はい、共感サウンド一個ゲットです。目覚ましの音、これはコンテンツになりそうです。
いつも聞いている「ジリリリ」という音を「コケコッコー」にしてみる、または「パオーン」と象の鳴き声にしてみる。いろんな動物の鳴き声をコンテンツにした「どうぶつ目覚ましスキル」というナイススキルが生まれそうです。

//blankline
でもA君はまだ起きません。するとお母さんが@<b>{「何度起こしたと思ってるの！遅刻するわよ！」}と鬼の形相で叫びます。思わずA君も飛び起きます。この会話も共感サウンドです。こんなシーン、誰でも経験してますよね。たとえば、ジリリリと目覚ましが鳴って、早く起きないとお母さんに叱られるドキドキ系ゲームは共感を呼びそうです。

//blankline
A君が起きるとテレビがついていました。朝の情報番組のCMに行く前に、時計のキャラクターが@<b>{「ろくじよんじゅうごふん、ろくじよんじゅうごふん」}と言いました。ああ、これも共感サウンドに使えるかもしれません。

//indepimage[clock][][scale=0.7]{
//}


A君は、朝ごはんを流し込んで、家を出ました。家を出てすぐに友達のケンタ君に出会いました。時間に余裕があったので、二人でグリコをしながら学校に向かいます。@<b>{「ジャンケンポン。グーリーコー」}はい、このあたりも共感サウンドですね。グリコのゲームシステムに注目した音声ゲームは子供達に受け入れられそうです。

//blankline
やがて二人は路地を出て、大通りを渡ります。横断歩道で信号待ちをしていると、信号から@<b>{「カッコー、カッコー」}と聞こえました。信号が変わるとそれは@<b>{「ピヨ、ピヨ」}という音に変わりました。目が不自由な方向けの音響装置ですね。
これも共感サウンドに使えそうです。目の不自由な人向けのコンテンツのナビゲーションに使うことができそうですし、カッコー、ピヨピヨという音を頼りにダンジョンをさまようサウンドゲーム、みたいな活用方法もあるかもしれません。

//blankline
学校につくと同時に@<b>{「キーンコーンカーンコーン」}、チャイムがなりました。これはかなり強力な共感サウンドです。誰もがすぐにイメージできますよね。たとえば学習スキルのジングルに入れれば記憶に残るでしょうし、「学校クイズ」のようなエンタメスキルのキーサウンドにもなりそうです。

//indepimage[school_building][][scale=0.7]{
//}


いかがでしょうか？A君が朝起きて学校にたどり着くまでに、いろいろなサウンドに触れていることに気づきましたでしょうか。ここまでに6種類の共感サウンドが見つかりました。もっと注意深く観察すればさらに見つかるはずです。

==== 20代男性サラリーマンのBさんの場合
次に大人の行動にも着目してみましょう。20代の男性サラリーマンのBさんです。
まず、目覚ましの音で目が覚めるのは同じです。とすれば目覚ましコンテンツは子どもから大人まで楽しめるものにできそうですね。

//blankline
起きて顔を洗い、ヒゲを剃ります。電動カミソリが@<b>{「ヴィイイイン」}と音を立てます。ここ、共感サウンドですね。大人の男性なら誰もがこの音に馴染みがあるでしょう。ここから毎朝のメンズの肌のお手入れスキルのようなコンテンツが浮かびます。

//blankline
Bさんが家を出ようとしたら@<b>{「ワンワン」}というイヌの鳴き声が聞こえました。ペットのポメラニアンに餌をあげるのを忘れていました。ペットの鳴き声も、共感サウンドですよね。
ネコやイヌの鳴き声スキルは定番です。ボイスアップラボでも「ネコ時計」というスキルをリリースしていますが、ネコの鳴き声と時計を掛け合わせるというコンテンツで、人気を博しています。

//indepimage[pets][][scale=0.7]{
//}


Bさんは家を出て、駅に向かいます。ポツポツポツ、雨が降ってきました。朝のテレビで天気予報を見ていたBさんは持っていた傘を広げます。@<b>{「ザーザーザー」}激しい雨の音が響きます。この音は強い共感サウンドですよね。「雨の音」という環境音スキルは世界中で人気があります。

//blankline
その後Bさんは 駅の手前でパトカーとすれ違いました。@<b>{「ウーウー、そこのバイク止まりなさい！」}どうやらバイクがスピード違反で捕まったようです。これも誰もが共感できるサウンドです。「そこのバイク止まりなさい」を別のワードに変えるだけで、愉快な大喜利スキルが生まれそうです。
パトカーのサイレンだけでなく、救急車や消防車のサイレンも共感度が高いと言えるでしょう。ドップラー効果に注目した面白いスキルも作れそうです。

//blankline
駅に着いたBさんは満員電車に乗ります。駅員のアナウンスが響きます。@<b>{「駆け込み乗車はおやめください。駆け込み乗車はおやめください。プルルルルルル（発車ベル）」}このセリフも共感サウンドですね。 発車ベルと合わせてネタ系スキルや、ゲームに使えそうです。

//indepimage[conductor][][scale=0.7]{
//}


と、まだまだ見つけられそうですが、ここまでにしておきましょう。
いかがでしょうか？意識するだけでいろんな共感サウンドを集めることができます。決して難しい作業ではありません。あなたもサウンドバスを使って、コンテンツの核となる共感サウンドをたくさん見つけ出してください。

==== 音声素材から見つける方法
サウンドを絡めたスキル作りに慣れていない人や、サウンドバスをやってもピンと来ない人は「音声素材サイト」を当たってみるのがおすすめです。
よっぽどサウンド編集に慣れている人でないかぎり、録音音声を使うのはハードルが高いですよね。となると、実際にスキルに使うのは既存の音声素材になります。次のAlexaオフィシャルのサウンドライブラリなどはよく使われています。ページ内でサウンドをプレビューできるため、スピーディに検索できて便利です。

//clearpage

//quote{
//noindent
@<b>{Alexa Skills Kitサウンドライブラリ}

//noindent
@<href>{https://developer.amazon.com/ja-JP/docs/alexa/custom-skills/ask-soundlibrary.html}
//}

//image[library][Alexa Skills Kit サウンドライブラリ][scale=0.7]{
//}


通常これらのライブラリは、演出のイメージが固まった後に利用するものですが、ここではスキルの構想段階のアイデアや共感サウンド探しに使ってみます。
やりかたはサウンドバスと一緒です。まず、ターゲットとなるオーディエンスを強く思い浮かべてみてください。そして、彼らが反応するであろうサウンドを見つけるのです。
オーディエンスを小学生としてライブラリを見てみます。試してみますと、

//source{
クイズショー、ジェットコースター、スイッチレバー、スポーツ／ホイッスル、ドアの軋む音、ドアのノック音、ホラー、エイリアン
//}

//noindent
ざっとこのあたりが共感サウンドとして引っかかりました。どのサウンドも小学生が聞くと「おっ」と思うでしょうし、色々な展開を生み出せそうです。

=== 共感サウンドからVUIコンテンツを紡ぎ出す
さて、あなたのオーディエンスにささる「共感サウンド」は見つかりましたか？
良いサウンドに出会いビビっときた方は、早速スキルづくりをはじめているかもしれません。しかし一方でまだピンときていない方もいると思います。
「結局、効果音をいっぱい使ったスキルを作れってこと？」と思われた方もいるかもしれませんので、ちょっと基本にもどって、コンテンツの定義をおさらいしますと、

//memo{
//noindent
@<b>{1. コンテンツとは、そのことについて意味を見出し楽しむ人々（＝オーディエンス）がいる事柄}

@<b>{2. オーディエンスとは「共感ポイント」を共有するグループのこと}
//}

//noindent
というものでした。「共感ポイントをサウンドにフォーカスすべし」と言ったのは、VUIのコア体験が @<b>{「サウンドや会話」} であるからです。日常生活でよく接しているサウンドや会話をフックにして「そうそうコレコレ」と共感してもらえば、少ない起動回数で「次も試したい」と感じてもらえる可能性が高くなるだろうと考えています。

//blankline
ただ見つけた共感サウンドを適当に挿入するだけでは良いコンテンツにはなりません。良いコンテンツに仕上げるためには、サウンドとスピーチをうまく絡める必要があります。その方法を、実例を交えながら見ていきましょう。

==== いきなり効果音スタートは効果的
良さげなサウンドは見つかったが「どう使えばよいかわからない」というエンジニアの方も多いと思います。そんな人は「効果音でスタートするスキル」を作ってみることをおすすめします。たとえば、こんなルールを入れてみましょう。

//source{
・スキルが起動された瞬間に効果音が流れる。そして何らかのセリフを入れる。
//}

試しにサウンドラブラリで見つけた「エイリアン」のサウンドで考えてみます。
スキルが起動した瞬間にエイリアンのうめき声を入れて、その後にこんなセリフを入れてみます。

//source{
@<b>{ユーザー}：「エイリアンゲームを開いて」
@<b>{効果音}：「ググワアワワアアア（エイリアンの声）」
@<b>{Alexa}：「エイリアンが地球にやってきて１週間、とうとうあなたはこの街での唯一の生き残りとなった。街を脱出するためには、目の前のサイコロで４以上の目を出さないといけない…」
//}

どうでしょうか。ゲームの説明ではじまるより、うんとドラマチックになったと思いませんか？こうするだけで初回起動時からユーザーをスキルに引き込み、何度も起動したいコンテンツになります。

//blankline
これはゲームに限らずワンショットスキルなどにも応用がききます。たとえば、ボイスアップラボがリリースしている「学校あるあるニュース」は共感サウンドではじまるワンショットスキルです。

//image[school][学校あるあるニュース][scale=0.7]{
//}


このスキルの流れは次のとおりです。

//source{
@<b>{ユーザー}：「学校あるあるニュースを開いて」
@<b>{効果音}：「ピロロロン、ピロロロン（緊急速報の音）」
@<b>{Alexa}：「緊急ニュースです！３組のY君が、先生に向かって「お母さん」と言ってしまったようです。」
//}

このスキルの共感サウンドは「緊急速報の音」です。TVを見ていると突然「ピロロロン！」と流れてテロップが表示される、誰もが経験するあの場面を再現します。「えっ、どんな事件があったの？」と思わせたらこっちのもの。次のセリフで「3組のY君がお母さんと言ってしまったようです」と超ユルい「あるあるネタ」を続けます。期待を裏切る「ギャップの面白さ」でユーザーを惹きつけようというねらいです。

//blankline
馴染みのあるサウンドを起点にして展開を考えることで、ユーザーの共感を得つつ、それを利用したコンテンツを作ることができます。「やられた」「面白い」と感じたユーザーの脳裏には、このスキルは残り続けるでしょうし、再び起動してもらえる確率も高まるでしょう。発想を変えて、もしも職場ネタだったらどんな音だろう？もしも居酒屋ネタだったらどんな音だろう？共感サウンドを起点にするだけで、様々なバリエーションのコンテンツを生み出すことができそうだと思いませんか？

==== 共感サウンド × サービス を掛け合わせる
もうひとつ、ボイスアップラボのワンショットスキル「ネコ時計」をご紹介させてください。このスキルはユーティリティ部門でランキング上位に常駐する人気スキルです。

//image[catclock][ネコ時計][scale=0.7]{
//}


このスキルのシナリオは次のとおりです。

//source{
@<b>{ユーザー}：「ネコ時計を開いて」
@<b>{Alexa}：「ネコたちが現在の時刻をお知らせします。」
@<b>{効果音}：「プップップッ（時報番号117の時報前の音）」
@<b>{効果音}：「ニャー、ニャー、ニャー…（ネコの声で時報）」
//}

このスキルを作るにあたり、ネコの鳴き声を共感サウンドに使うことは決めていました。しかし、ただネコがニャーニャー鳴くだけのスキルはすでに複数あるので差別化できない。そこで「時計」と絡めることにしました。ここでさらに「時報番号117」という別のファクターが登場します。117にかけると事務的な声で「ただいまより9時30分をおしらせします。プップップッポーン」というガイダンスが流れますよね。30代以上にはなじみのある音声です。これも共感サウンドとして使うことにしました。

//blankline
このスキルのポイントは、

//source{
@<b>{猫＆時報の共感サウンド × 時計}
//}

//noindent
という二つの要素を結びつけたところです。共感サウンドだけでも、時計だけでもインパクトがない。そんなときに有効なのが、

//source{
@<b>{共感サウンド × サービス（機能）}
//}

//noindent
の掛け合わせで考える方法です。こうすることで、あらゆるタイプのスキルに応用することが可能になります。
サービスをそのまま実装するだけでは面白くない。そんなときに共感サウンドの力を借りれば@<b>{「世界観あるコンテンツ」}に進化させることができるのです。

===== 天気予報スキルを考えてみる
@<raw>{　\n}

たとえば、天気予報のスキルを作るとしましょう。
今スキルストアにある天気予報は、起動すると「東京都新宿区の今日の天気は雨です。降水確率は…」のようにアナウンスしてくれるタイプがメインですよね。どれも必要十分なスキルです。激戦区であるこのジャンルで目立つためにはどんな内容にすれば良いでしょうか？

//indepimage[weather][][scale=0.7]{
//}


エンジニア的に攻めるなら、たとえば1時間ごとの天気を聞けるとか、雨雲レーダーの様子を分かりやすく伝える、というような「機能性」でアピールするのが普通でしょう。これも悪くなさそうです。
ですが角度を変えて共感サウンドで差別化を加えることを考えてみましょう。

//blankline
僕だったら、たとえばその日の予想が雨ならば、起動してすぐに「雨の音」を流したいと考えます。つまり、

//source{
@<b>{雨のサウンド × 天気予報サービス}
//}

//noindent
という掛け合わせでコンテンツ化を考えます。こんなシナリオです。

//source{
@<b>{ユーザー}：「サウンド天気を開いて」
@<b>{効果音}：「ポツポツポツ、ザーザーザー（雨が降り出す音）」
@<b>{Alexa}：「おや、今日の天気は下り坂のようですね…」
//}

降水確率によって雨の音の強弱をつけるのも良いでしょう。「ポツポツ」か「ザーザー」かで違いをあらわすことはできそうです。「雨のち晴れ」ならば、次第に雨がやんで、太陽が出てくる様子を10秒程度のサウンドトラックにして表現したいところです。季節によってはカエルの鳴き声など、変化をつけるのも良いですね。はたまた、女の子のキャラクターで「雨の日なんて大嫌い。でも今日は午後から晴れそう。」と言わせるのも良さげです。

//blankline
大事なのは、そのサウンドに共感できて印象的であること。一度聞いたら次の日も聞きたくなる、そんなユーザー体験を目指したいところです。こうすることで、ただの天気予報を「ファンを生み出すコンテンツ」に仕上げることも可能になります。

==== エンタメコンテンツの核として
共感サウンドを使ったコンテンツ作りに一番相性が良いのはやはりエンターテイメント系スキルでしょう。
共感という部分をうまく使うことができれば、そのコンテンツに納得感が生まれたり、良い意味で裏切った展開もやりやすくなります。

===== 電話の音に着目したホラーゲーム
@<raw>{　\n}

ここで、そんな共感サウンドの特性を活かした、ボイスアップラボの人気スキル「ゾンビのまち」を紹介させてください。

//image[zombie][ゾンビのまち][scale=0.7]{
//}


このスキルは音声による謎解きゲームです。主人公の少年を音声でナビゲートして、ゾンビがはびこる町から救い出すというストーリーで、APL @<fn>{apl} を使ったビジュアル要素も大きな特徴になっています。リリース当初から順調にユーザーを増やし、またリピート率も高い人気コンテンツになっています。
//footnote[apl][Alexa Presentation Language = アマゾンが提供する、画面を持つAlexaデバイスを制御するためのデザイン言語。 ]

//blankline
もともと「謎解きゲーム」を音声でやる、ということだけ決まっていました。ヒントの一文を参考に、謎解き問題を解いていくという、あっさりとしたゲームの構成を考えていたのです。
ただ、それではストレートすぎてユーザーに楽しんでもらえないので、何か良いアイデアはないかなと考えていました。ある時、仕事の電話をかけていたのですが、呼び出しの時の「プルルルル」という音を聞いて「あっ」と思ったのです。これを共感サウンドに使ってみようと思いました。掛け合わせとしては、

//source{
@<b>{電話の音 × 謎解きゲーム}
//}

//noindent
ですね。で、完成したゾンビのまちの起動シナリオはこちらです。

//source{
@<b>{ユーザー}：「ゾンビのまちを開いて」
@<b>{効果音}：「（電話の音）プルルルル、プルルルル、プルル…ガチャ」
@<b>{主人公}：「もしもし、君は、だれ？　助けて欲しいんだ」
//}

スキルを起動するイコール「電話をかける」ということに決めて、その後の起動時もすべて統一しました。
さらに主人公の携帯電話を電池残量が少ない設定とし、電話の電源を切っている時（移動時など）は次のように、スキルを起動してもつながらないというギミックを入れました。

//source{
@<b>{ユーザー}：「ゾンビのまちを開いて」
@<b>{効果音}：「（電話の音）プーッ、プーッ、プーッ」
@<b>{Alexa}：「おかけになった電話は、電源が入っていないようです。」
//}

このスキルのすべてのスタート地点は「電話の音」です。誰もが共感でき、説明もいらないこのサウンドを発見できたことから、ゾンビという設定や主人公のキャラクター、登場人物や、ストーリー展開まで発想を膨らませることができたのです。

//indepimage[zombie_man][][scale=0.7]{
//}


===== パンチやキック音に着目した格闘ゲーム
@<raw>{　\n}

他にも、たとえば「ボイスカンフー」というスキルでは、音声では難しい「格闘ゲーム」を、

//source{
@<b>{パンチやキックの音 × ジャンケン}
//}

//noindent
の掛け合わせでコンテンツ化しました。

//image[kungfu][ボイスカンフー][scale=0.7]{
//}


パンチやキックのサウンドは、格闘ゲームでおなじみです。でもそれを音声のみで実現するのは難しい。そこで「ジャンケン」の仕組みと組み合わせることにしました。パンチ（キック）、守る、投げる、をジャンケンのような関係性にし、且つ、一度に複数のジャンケンに対応するというシステムで、アクションゲームのようなハラハラ感を演出しています。

===== 心地よい環境音に着目した放置ゲーム
@<raw>{　\n}

また、最新作の「こびとのせかい」では、Alexaスキルの中でも需要の高い「環境サウンド」をゲームに取り入れました。掛け合わせたのは、

//source{
@<b>{心地よい採掘サウンド × 放置ゲーム}
//}

//noindent
です。この二つを掛け合わせることで、「毎日聞きたくなる」ゲームを目指しました。

//image[pygmy][こびとのせかい][scale=0.7]{
//}


このスキルでまずやりたかったのは「マインクラフト」の音声版のようなゲームでした。マインクラフトはサウンドが秀逸で、プレイサウンドをひたすら流すだけの Youtube も人気です。心地よい「採掘サウンド」をゲーム化するために、もう一つのアイデア「放置ゲーム」を取り入れました。プレイヤーはリアルタイムでキャラクターを操作するのではなく、道具を買ったり、休憩を与えたりするのみで、あとは自動的にゲームが進む、というレイジーなシステムを採用しています。

//blankline
これらの例のように、「共感サウンド × ゲームシステム」をうまく掛け合わせることで、ユーザーが没頭するエンターテイメントに仕上げることができます。ぜひ参考にしてください。

//blankline
あなたも良い掛け合わせを見つけて、ユーザーをハッとさせるようなエンタメコンテンツを作ってみてくださいね。

//clearpage
== まとめ
さて、共感サウンドによるコンテンツづくりの紹介は以上です。いかがでしたか？しっくりときましたか？
最後に、これまでのコンテンツ作りのポイントをまとめたいと思います。

//blankline
まず、コンテンツの定義は、

//memo{
//noindent
@<b>{1. コンテンツとは、そのことについて意味を見出し楽しむ人々（＝オーディエンス）がいる事柄}

@<b>{2. オーディエンスとは「共感ポイント」を共有するグループのこと}
//}

そして、VUIコンテンツをつくるなら「共感サウンド」を見つけることが近道。
共感サウンドを見つけるための方法は、オーディエンスをイメージしながら、

//source{
@<b>{1. サウンドバスをやってみる}
@<b>{2. サウンドライブラリを探ってみる}
//}

//noindent
などが有効です。そして共感サウンドが見つかったら、コンテンツ化するのですが、そのときの切り口として、

//source{
@<b>{1. いきなり効果音スタートのスキルを作ってみる}
@<b>{2. 共感サウンド × サービス を掛け合わせる}
@<b>{3. エンタメコンテンツの核にしてみる}
//}

//noindent
などを試してみると、効果的なコンテンツになると思います。

== おわりに

ふう、本当はもう少しテクニカルなことも書こうと思っていたのですが、そこまでたどり着くことができませんでした。いやー文章書くのって大変ですね。

//blankline
さて、この章の内容はいかがでしたか？ここで紹介した手法がコンテンツ作成のすべてではありませんし、すべてのタイプのスキルに応用できるわけでもありません。ですが、良質な音声コンテンツを作ろうと思うクリエイターさんなら、みな通る道ではないかと思っています。

//blankline
少なくとも僕がいつもやっているコンテンツ作りの一端はお伝えできたと思います。なるほど、こんな考え方もあるんだと知っていただいたり、この手法に改良を加えて自分なりに試していただけたらこれほど嬉しいことはありません。

//blankline
そうそう、これを読んでもしもボイスアップラボのスキルに興味が湧いてきましたら、ぜひサイト（@<href>{https://voiceapp-lab.com/}）にもお立ち寄りください。ここでは紹介できなかったいろいろなスキルを試していただけます。参考になるものもあれば、ならないものもありますが、我々のチャレンジの全てを見ていただけると思います。

//blankline
ではでは、あなたが作る素敵なスキルに出会えることを楽しみにして。

//blankline
2019.12.31 マクドナルド京都北白川店にて　コバヤシトール

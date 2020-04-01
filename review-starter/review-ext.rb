# -*- coding: utf-8 -*-

##
## Re:VIEWを拡張し、インライン命令とブロック命令を追加する
##


require_relative './lib/hooks/monkeypatch'  ## 諸々の修正（モンキーパッチ）


module ReVIEW

  ## インライン命令「@<clearpage>{}」を宣言
  Compiler.definline :clearpage         ## 改ページ
  Compiler.definline :nop               ## 引数をそのまま表示 (No Operation)
  Compiler.definline :letitgo           ## （nopのエイリアス名）
  Compiler.definline :B                 ## @<strong>{}のショートカット
  Compiler.definline :foldhere          ## 折り返し箇所を手動で指定
  Compiler.definline :hearts            ## ハートマーク
  Compiler.definline :TeX               ## TeX のロゴマーク
  Compiler.definline :LaTeX             ## LaTeX のロゴマーク
  Compiler.definline :cursor            ## ターミナルでのカーソル
  Compiler.definline :weak              ## 目立たせない（@<strong>{} の反対）
  Compiler.definline :small             ## 文字サイズを小さく
  Compiler.definline :xsmall            ## 文字サイズをもっと小さく
  Compiler.definline :xxsmall           ## 文字サイズをもっともっと小さく
  Compiler.definline :large             ## 文字サイズを大きく
  Compiler.definline :xlarge            ## 文字サイズをもっと大きく
  Compiler.definline :xxlarge           ## 文字サイズをもっともっと大きく
  Compiler.definline :xstrong           ## 文字を大きくした@<strong>{}
  Compiler.definline :xxstrong          ## 文字をもっと大きくした@<strong>{}

  ## ブロック命令「//textleft{ ... //}」等を宣言
  ## （ここでは第2引数が「0」なので、引数なしのブロック命令になる。
  ##   もし第2引数が「1..3」なら、//listのように必須引数が1つで
  ##   非必須引数が2という意味になる。）
  Compiler.defblock :textleft, 0        ## 左寄せ
  Compiler.defblock :textright, 0       ## 右寄せ
  Compiler.defblock :textcenter, 0      ## 中央揃え
  Compiler.defblock :abstract, 0        ## 章の概要
  Compiler.defblock :makechaptitlepage, 0..1, true ## 章タイトルを独立したページに
  #
  Compiler.defblock :list, 0..3         ## （上書き）
  Compiler.defblock :listnum, 0..3      ## （上書き）
  #
  Compiler.defsingle :clearpage, 0      ## 改ページ (\clearpage)
  Compiler.defsingle :sampleoutputbegin, 0..1 ## （出力結果開始部、Starterドキュメントで使用）
  Compiler.defsingle :sampleoutputend, 0      ## （出力結果終了部、Starterドキュメントで使用）


  ## LaTeX用の定義
  class LATEXBuilder

    ## 改ページ（インライン命令）
    def inline_clearpage(str)
      '\clearpage'
    end

    ## 改ページ（ブロック命令）
    def clearpage()
      puts ''
      puts '\\clearpage'
      puts ''
    end

    ## 引数をそのまま表示
    ## 例：
    ##   //emlist{
    ##     @<b>{ABC}             ← 太字の「ABC」が表示される
    ##     @<nop>$@<b>{ABC}$ ← 「@<b>{ABC}」がそのまま表示される
    ##   //}
    def inline_nop(str)
      escape(str)
    end
    alias inline_letitgo inline_nop

    ## @<strong>{} のショートカット
    def inline_B(str)
      inline_strong(str)
    end

    ## @<b>{} が //terminal{ ... //} で効くように上書き
    def inline_b(str)
      ##macro('textbf', escape(str))   # original
      if within_codeblock?()
        #"{\\bfseries #{escape(str)}}"            # \seqsplit{} 内では余計な空白が入る
        #"{\\bfseries{}#{escape(str)}}"           # \seqsplit{} 内では後続も太字化する
        "\\bfseries{}#{escape(str)}\\mdseries{}"  # \seqsplit{} 内でうまく効く
      else
        macro('textbf', escape(str))
      end
    end

    ## @<del>{} が //list や //terminal で効くように上書き
    def inline_del(str)
      ##macro('reviewstrike', escape(str))   # original
      if within_codeblock?()
        #"\\reviewstrike{#{escape(str)}}"    # \seqsplit{} 内でエラーになる
        #"{\\reviewstrike{#{escape(str)}}}"  # \seqsplit{} 内でもエラーにならないが折り返しされない
        "{\\reviewstrike{\\seqsplit{#{escape(str)}}}}"  # エラーにならないし、折り返しもされる
      else
        macro('reviewstrike', escape(str))
      end
    end

    ## 目立たせない（@<strong>{} の反対）
    def inline_weak(str)
      if within_codeblock?()
        "{\\starterweak{\\seqsplit{#{escape(str)}}}}"
      else
        "\\starterweak{#{escape(str)}}"
      end
    end

    ## 文字を小さくする
    def inline_small(str)   ; "{\\small{}#{escape(str)}}"       ; end
    def inline_xsmall(str)  ; "{\\footnotesize{}#{escape(str)}}"; end
    def inline_xxsmall(str) ; "{\\scriptsize{}#{escape(str)}}"  ; end

    ## 文字を大きくする
    def inline_large(str)   ; "{\\large{}#{escape(str)}}" ; end
    def inline_xlarge(str)  ; "{\\Large{}#{escape(str)}}" ; end
    def inline_xxlarge(str) ; "{\\LARGE{}#{escape(str)}}" ; end

    ## 文字を大きくした@<strong>{}
    def inline_xstrong(str) ; "{\\Large{}#{inline_strong(str)}}" ; end
    def inline_xxstrong(str); "{\\LARGE{}#{inline_strong(str)}}" ; end

    ## コードブロック中で折り返し箇所を手動で指定する
    ## （\seqsplit による自動折り返し機能が日本語には効かないので、
    ##   長い行を日本語の箇所で折り返したいときは @<foldhere>{} を使う）
    def inline_foldhere(arg)
      return '\starterfoldhere{}'
    end

    ## ハートマーク
    def inline_hearts(str)
      '$\heartsuit$'
    end

    ## TeXのロゴマーク
    def inline_TeX(str)
      '\TeX{}'
    end

    ## LaTeXのロゴマーク
    def inline_LaTeX(str)
      '\LaTeX{}'
    end

    ## ターミナルでのカーソル（背景が白、文字が黒）
    def inline_cursor(str)
      "{\\startercursor{#{escape(str)}}}"
    end

    ## 左寄せ
    def textleft(lines)
      puts '\begin{flushleft}'
      puts lines
      puts '\end{flushleft}'
    end

    ## 右寄せ
    ## （注：Re:VIEWにはすでに //flushright{ ... //} があったので、今後はそちらを推奨）
    def textright(lines)
      puts '\begin{flushright}'
      puts lines
      puts '\end{flushright}'
    end

    ## 中央揃え
    ## （注：Re:VIEWにはすでに //centering{ ... //} があったので、今後はそちらを推奨）
    def textcenter(lines)
      puts '\begin{center}'
      puts lines
      puts '\end{center}'
    end

    ## 導入文（//lead{ ... //}）のデザインをLaTeXのスタイルファイルで
    ## 変更できるよう、マクロを使う。
    def lead(lines)
      puts '\begin{starterlead}'   # オリジナルは \begin{quotation}
      puts lines
      puts '\end{starterlead}'
    end

    ## 章 (Chapter) の概要
    ## （導入文 //lead{ ... //} と似ているが、導入文では詩や物語を
    ##   引用するのが普通らしく、概要 (abstract) とは違うみたいなので、
    ##   概要を表すブロックを用意した。）
    def abstract(lines)
      puts '\begin{starterabstract}'
      puts lines
      puts '\end{starterabstract}'
    end

    ## 章タイトルを独立したページに
    def makechaptitlepage(lines, option)
      case option
      when nil, ""  ;
      when 'toc=section', 'toc=subsection' ;
      when 'toc'
      else
        raise ArgumentError.new("//makechaptitlepage[#{option}]: unknown option (expected 'toc=section' or 'toc=subsection').")
      end
      puts "\\makechaptitlepage{#{option}}"
    end

    ## 引用（複数段落に対応）
    ## （入れ子対応なので、中に箇条書きや別のブロックを入れられる）
    def on_quote_block()
      puts '\begin{starterquote}'
      yield
      puts '\end{starterquote}'
    end
    def quote(lines)
      on_quote_block() do
        puts lines
      end
    end

    ## 引用 (====[quote] ... ====[/quote])
    ## （ブロック構文ではないので、中に箇条書きや別のブロックを入れられる）
    def quote_begin(level, label, caption)
      puts '\begin{starterquote}'
    end
    def quote_end(level)
      puts '\end{starterquote}'
    end

    ## ノート（//note[caption]{ ... //}）
    ## （入れ子対応なので、中に箇条書きや別のブロックを入れられる）
    def on_note_block(caption=nil)
      s = compile_inline(caption || "")
      puts "\\begin{starternote}{#{s}}"
      yield
      puts "\\end{starternote}"
    end
    def note(lines, caption=nil)
      on_note_block(caption) do
        puts lines
      end
    end

    ## ノート (====[note] ... ====[/note])
    ## （ブロック構文ではないので、中に箇条書きや別のブロックを入れられる）
    def note_begin(level, label, caption)
      enter_context(:note)
      s = compile_inline(caption || "")
      puts "\\begin{starternote}{#{s}}"
    end
    def note_end(level)
      puts "\\end{starternote}"
      exit_context(:note)
    end

    ## コードリスト（//list, //emlist, //listnum, //emlistnum, //cmd, //source）
    ## TODO: code highlight support
    def list(lines, id=nil, caption=nil, lang=nil)
      program(lines, id, caption, _codeblock_optstr(lang, false))
    end
    def listnum(lines, id=nil, caption=nil, lang=nil)
      program(lines, id, caption, _codeblock_optstr(lang, true))
    end
    def emlist(lines, caption=nil, lang=nil)
      program(lines, nil, caption, _codeblock_optstr(lang, false))
    end
    def emlistnum(lines, caption=nil, lang=nil)
      program(lines, nil, caption, _codeblock_optstr(lang, true))
    end
    def source(lines, caption=nil, lang=nil)
      program(lines, nil, caption, _codeblock_optstr(lang, false))
    end
    def cmd(lines, caption=nil, lang=nil)
      terminal(lines, nil, caption, _codeblock_optstr(lang, false))
    end
    def _codeblock_optstr(lang, lineno_flag)
      arr = []
      arr << lang if lang
      if lineno_flag
        first_line_num = line_num
        arr << "lineno=#{first_line_num}"
        arr << "linenowidth=0"
      end
      return arr.join(",")
    end
    private :_codeblock_optstr

    ## 出力結果の開始部と終了部（Starterのドキュメントで使用）
    ## （Re:VIEWではブロックの入れ子も「===[xxx]」の入れ子もできないため）
    def sampleoutputbegin(caption=nil)
      #puts "\\begin{startersampleoutput}"   # error in note block
      s = caption ? compile_inline(caption) : nil
      puts "\\startersampleoutput{#{s}}"
    end
    def sampleoutputend()
      #puts "\\end{startersampleoutput}"     # error in note block
      puts "\\endstartersampleoutput"
    end

  end


  ## HTML（ePub）用の定義
  class HTMLBuilder

    ## 改ページはHTMLにはない
    def inline_clearpage(str)   # インライン命令
      puts '<p></p>'
      puts '<hr />'
      puts '<p></p>'
    end

    def clearpage()             # ブロック命令
      puts '<p></p>'
      puts '<hr />'
      puts '<p></p>'
    end

    ## 引数をそのまま表示 (No Operation)
    def inline_nop(str)
      escape_html(str)
    end
    alias inline_letitgo inline_nop

    ## @<strong>{} のショートカット
    def inline_B(str)
      inline_strong(str)
    end

    ## 目立たせない（@<strong>{} の反対）
    def inline_weak(str)
      "<span class=\"weak\">#{escape_html(str)}</span>"
    end

    ## 文字を小さくする
    def inline_small(str)   ; "<small style=\"font-size:small\">#{escape(str)}</small>"   ; end
    def inline_xsmall(str)  ; "<small style=\"font-size:x-small\">#{escape(str)}</small>" ; end
    def inline_xxsmall(str) ; "<small style=\"font-size:xx-small\">#{escape(str)}</small>"; end

    ## 文字を大きくする
    def inline_large(str)   ; "<span style=\"font-size:large\">#{escape(str)}</span>"   ; end
    def inline_xlarge(str)  ; "<span style=\"font-size:x-large\">#{escape(str)}</span>" ; end
    def inline_xxlarge(str) ; "<span style=\"font-size:xx-large\">#{escape(str)}</span>"; end

    ## 文字を大きくした@<strong>{}
    def inline_xstrong(str) ; "<span style=\"font-size:x-large\">#{inline_strong(str)}</span>"; end
    def inline_xxstrong(str); "<span style=\"font-size:xx-large\">#{inline_strong(str)}</span>"; end

    ## コードブロック中で折り返し箇所を手動で指定する
    def inline_foldhere(arg)
      '<br>'
    end

    ## ハートマーク
    def inline_hearts(str)
      #'&hearts;'
      '&#9829;'
    end

    ## TeXのロゴマーク
    def inline_TeX(str)
      'TeX'
    end

    ## LaTeXのロゴマーク
    def inline_LaTeX(str)
      'LaTeX'
    end

    ## ターミナルでのカーソル（背景が白、文字が黒）
    def inline_cursor(str)
      "<span class=\"cursor\">#{escape_html(str)}</span>"
    end

    ## 左寄せ
    def textleft(lines)
      puts '<div style="text-align:left">'
      puts lines
      puts '</div>'
    end

    ## 右寄せ
    def textright(lines)
      puts '<div style="text-align:right">'
      puts lines
      puts '</div>'
    end

    ## 中央揃え
    def textcenter(lines)
      puts '<div style="text-align:center">'
      puts lines
      puts '</div>'
    end

    ## 章 (Chapter) の概要
    def abstract(lines)
      puts '<div class="abstract">'
      puts lines
      puts '</div>'
    end

    ## 章タイトルを独立したページに
    def makechaptitlepage(lines)
      puts ''   # HTMLでは特に何もしない
    end

    ## 引用（複数段落に対応）
    def blockquote(lines)
      puts '<blockquote class="blockquote">'
      puts lines
      puts '</blockquote>'
    end

    ## 引用（//quote{ ... //}）
    ## （入れ子対応なので、中に箇条書きや別のブロックを入れられる）
    def on_quote_block()
      puts '<blockquote class="blockquote">'
      yield
      puts '</blockquote>'
    end
    def quote(lines)
      on_quote_block() do
        puts lines
      end
    end

    ## 引用 (====[quote] ... ====[/quote])
    ## （ブロック構文ではないので、中に別のブロックや箇条書きを入れられる）
    def quote_begin(level, label, caption)
      puts '<blockquote class="blockquote">'
    end
    def quote_end(level)
      puts '</blockquote>'
    end

    ## ノート（//note{ ... //}）
    ## （入れ子対応なので、中に箇条書きや別のブロックを入れられる）
    def on_note_block(caption=nil)
      puts "<div class=\"note\">"
      puts "<h5>#{caption}</h5>" if caption.present?
      yield
      puts "</div>"
    end
    def note(lines, caption=nil)
      on_quote_block(caption) do
        puts lines
      end
    end

    ## ノート (====[note] ... ====[/note])
    ## （ブロック構文ではないので、中に別のブロックや箇条書きを入れられる）
    def note_begin(level, label, caption)
      s = compile_inline(caption || "")
      puts "<div class=\"note\">"
      puts "<h5>#{s}</h5>" if s.present?
    end
    def note_end(level)
      puts "</div>"
    end

    ## 出力結果の開始部と終了部（Starterのドキュメントで使用）
    ## （Re:VIEWではブロックの入れ子も「===[xxx]」の入れ子もできないため）
    def sampleoutputbegin(caption=nil)
      puts "<div class=\"sampleoutput\">"
      if caption
        s = compile_inline(caption)
        puts "<span class=\"caption\">#{caption}</span>"
      end
      puts "<div class=\"sampleoutput-inner\">"
    end
    def sampleoutputend()
      puts "</div></div>"
    end

  end


  class TEXTBuilder

    ## TeXのロゴマーク
    def inline_TeX(str)
      'TeX'
    end

    ## LaTeXのロゴマーク
    def inline_LaTeX(str)
      'LaTeX'
    end

  end


end

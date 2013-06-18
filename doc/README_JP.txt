***** README_JP.txt --- RubyNetCDF とは?--- *****

*** RubyNetCDF の背景と目的 ***

今日、気象、大気、超高層科学などの分野に置いては、観測機器の発達により、
多くの観測データが得られるようになり、さらに計算機科学の発達により、計
算機シミュレーションによる数値解析がさかんに行われるようになった。その
結果、現在では各研究室単位で扱われるデータ量だけを考えても、その量はか
なり膨大なものとなり、少しでも、効率よくデータ処理を行う必要がでてきた。
さらに、観測データ、シミュレーションのデータを解析するためのプログラム
作成はファイルのフォーマットが違えば、逐一そのフォーマットに合わせて、
プログラムを作成せねばならず、非常に労力と時間を費やすことになる。これ
を解決するには、様々なフォーマットデータを統一して扱えるインターフェー
スが必要で、そのインタフェースは可視化や、数値計算にも適したものが必要
である。そのような背景をふまえて、開発効率が高く、プログラムの保守管理
が容易であるという理由からオブジェクト指向言語rubyを用いたデータ処理、
可視化環境開発が有効と考える。そのための種々のフォーマットデータを扱
うためのインターフェース作りの一つとして、自己記述型フォーマットである、
netCDFデータのCインターフェースにおけるrubyによるラッパーの開発を目的と
する。

*** netCDFについて ***

netCDFは、Univercity Corporation for Atmospheric Researchによって開発さ
れた。バイナリデータとの違いは、netCDFは単なる数値配列のならびになってい
るのではなく、その他の付加的情報をそのファイルの中に持っているということ
である。具体的な構造は、

file--has→dimenstions 
               ↑ ref 
    --has→variables→ attributes 

    --has→attributes (global) 

といった具合に、次元、変数名、属性という付加的情報を持ったフォーマットで
ある。

netCDFのインターフェースとしては、C言語用、FORTRAN言語用、C++言語用、
perl言語用、とあるが、rubyはもともとCでかかれており、さらに機能拡張をす
る場合は拡張ライブラリというプログラムをCで作成して実現することができる
ようになっている。そこでC言語インターフェースにrubyの側からアクセスでき
るように拡張ライブラリを作成する。

*** 実際に開発したプログラムの仕様 ***

まず、上記のようなnetCDFの構造に合わせて、新しく拡張ライブラリ中に
4つのクラスを定義した。

〇 NetCDFクラス
〇 NetCDFDim クラス
〇 NetCDFVar クラス
〇 NetCDFAtt クラス

このクラスのそれぞれにCインターフェースに合わせて作成したメソッドだけで
なく、NetCDFDim,NetCDFAttクラスのオブジェクトに関するイテレータが容易に
できるメソッドを用意し、変数へのデータの書込や、データの読み出しもハッシュ
を利用したオプション引数による制御を可能にすることで利用しやすいインター
フェースを実現した。またrubyの配列は数値計算に向いていないので、netCDF
ファイルに書き込む前、またnetCDFファイルから読み出したデータ配列の格納
には、数値計算に適した数値型配列NArrayを使用している。

一つ一つのメソッドの仕様については、netcdf-0.01.tar.gzに同梱されている
how_to_use_netcdf.docまたはhow_to_use_netcdf.pdfに記載されているのでそ
ちらを参照下さい。

*** インストール方法および注意事項等 ***

netcdf-0.01.tar.gzを展開してできるnetcdf-0.01/ディレクトリにある
extconf.rbを使用して、

ruby extconf.rb,
make
make test
make site-install(またはmake install)
でインストールできる。(make test はテストをするだけなのでしなくても良
い)

またこのプログラムを使用するためには、NArrayがあらかじめインストールさ
れていることが必要になる。またNArrayのヘッダファイル、 

narray.h,
narray_config.h

はrubyのヘッダファイルruby.hの格納場所
(通常/usr/local/lib/ruby/1.6/アーキテクチャ名)
以下にコピーしておく必要がある。

さらに、これは注意事項です。

ここで、extconf.rbの中では、netcdf-0.01/にあるmkmf.rbを呼び出して、
実行しますが、一部rubyのもともとのmkmf.rbに変更を加えています。
変更の仕方を以下に示します。今後、rubyのバージョンアップが行われ、
mkmf.rbが書き換えられた際の参考にしてください。

**********************************************************************
        install:       $(archdir)/$(DLLIB)

        site-install:  $(sitearchdir)/$(DLLIB)

        となっているので、$(archdir)/$(DLLIB): と
        $(sitearchdir)/$(DLLIB): にそれぞれ、 

        $(archdir)/$(DLLIB): $(DLLIB) のほうは
        
        @$(RUBY) -r ftools -e 'File::makedirs(*ARGV)' $(rubylibdir) $(archdir)
        @$(RUBY) -r ftools -e 'File::install(ARGV[0], ARGV[1], 0555,
        true)' $(DLLIB) $(archdir)/$(DLLIB)

        の直後に、

        @$(RUBY) -r ftools -e 'File::install(ARGV[0], ARGV[1], 0555, true)'
        netcdf.rb $(rubylibdir)/netcdf.rb

        を、

        $(sitearchdir)/$(DLLIB): $(DLLIB) のほうは

        @$(RUBY) -r ftools -e 'File::makedirs(*ARGV)' $(libdir) $(sitearchdir)
        @$(RUBY) -r ftools -e 'File::install(ARGV[0], ARGV[1], 0555,
        true)' $(DLLIB) $(sitearchdir)/$(DLLIB)

        の直後に、

        @$(RUBY) -r ftools -e 'File::install(ARGV[0], ARGV[1], 0555,
        true)' netcdf.rb $(sitelibdir)/netcdf.rb 

        を加えています。

	disclean: に対し、もとは
		@$(RM) core ruby$(EXEEXT) *~
	であるのに対し、
		@$(RM) core ruby$(EXEEXT) *~ *.nc demo/*.nc
	と、*.nc ファイルを消去するようにしました。
*************************************************************************

配布条件はRubyのライセンスに準拠する。

なお、netcdf,ruby,narrayに関する情報は以下のURLを参照下さい。

netcdfのホームページ

http://www.unidata.ucar.edu/packages/netcdf/

rubyのホームページ

http://www.ruby-lang.org/

narrayのホームページ

http://www.ruby-lang.org/en/raa-list.rhtml?name=NArray

***** End of the README_JP.txt *****

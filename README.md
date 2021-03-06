# Integer

Multiple Precision Arithmetic

長野高専の3J「アルゴリズムとデータ構造」後期の多倍長演算で作成したプログラム群です。
2016年の1月に出された全課題（N = 0..10）の回答付きです。
(2018年現在に) 少し見た感じだといくつかの課題内容は入れ替わっている（そして難度が上がっている）
ので参考程度に。

## 多倍長演算の高速化

このプログラムがどれくらい速いかと言うと、
自宅のパソコン（Core i7）を使って
ガウスのarctan系公式により π を求める課題で以下の実行時間を要します。

- πの小数点以下1000桁を求めるのに 0.3秒
- πの小数点以下10000桁を求めるのに 2分7秒

実際に与えられている課題は小数点以下1000桁までなので速度的には十分です。
以下はテキストの内容以上に高速化する方法について覚書しておきます。


### 基数の変更

各桁は long 型の配列で表し、基数（RADIX）は 10^9 にします。

テキストでも触れられていますが、基数を10として配列の1つの要素で 0~9 しか扱えないのは効率が悪いです。
テキストではint型しか説明していませんが、long型を使う場合は
long型の最大値がおよそ 9 * 10^18 なので、
桁同士の乗算をしても桁あふれしない 10^9 が基数として適当です。
以下の図は、基数を増やすだけで乗算回数が減る様子です。
基数を 10^1 から 10^2 にするだけで乗算回数が1/4に抑えられることが確認できます。

```
    RADIX = 10^1         RADIX = 10^2

           a b c d            ab cd
    *)     a b c d       *)   ab cd
    ---------------      -----------
           4 3 2 1             2  1
         8 7 6 5            4  3
       c b a 9
     g f e d

    step: 16             step: 4
```


### 小さい数で割る場合の除算アルゴリズム

多倍長で a/b を行う方法として、テキストや授業では3つの除算の方法を紹介しています。
- [除算アルゴリズム] b < a が成り立つ間、繰り返し引き算する
- [除算アルゴリズムの改良] b < a が成り立つ間 b を10倍する作業を繰り返してからまとめて引き算する
- [筆算方式の除算] a の上位の桁から右にずらしていき、b で引き算できた回数によって商の上位の桁から順に決定していく

しかし、どの方法も基数を増やしたときに恩恵を受けることができません。
加算、減算、乗算は基数を増やした分だけ処理回数が減っていますが、
除算は商の上位の桁から順々に決定していくために処理回数が減りません。
しかし、割る数 b が基数よりも小さい場合は1サイクルで基数の分だけ商の上位桁を決定することができます。

簡単なプログラムを次に示します。
ただし、2つの非負整数 a, b から基数 R の商 q と剰余 r を求めます。
i の初期値は a の最上位桁の位置です。

``` c
// a/bを筆算方式で求める。
// ただし、b が基数 RADIX より小さい場合にしか使えない。

num = a.n[i]; // 作業用変数num
for (; i >= 0; i--) {
    quot = num / b;
    rem  = num % b;
    q.n[i] = quot;
    if (i == 0) break;

    next = a.n[i-1];
    num = rem * RADIX + next;
}
r = rem;
```

しかし、この「小さい数で割る場合の除算アルゴリズム」は b < 基数 という条件付きなので、
処理速度のボトルネックを解消するには程遠いです。
次の節では全ての b で処理速度の向上が見込めるアルゴリズム「Kunth's Algorithm D」を紹介します。


### Kunth先生の除算アルゴリズム

多倍長で a/b を行うときに、Kunth's Algorithm D で除算をすると b の値の大きさに関係なく、
1サイクルで基数の分だけ商の桁を求めることができます。
つまり、基数が 10^9 であれば1サイクルで商の上位9桁が決定します。

Kunth's Algorithm D を元に改良したアルゴリズムの手順は次の通りです。
ただし、2つの非負整数 a, b から基数 R の商 q と剰余 r を求めます。
また、a の桁数を s, b の桁数を t とします。
例えば a = 123456 で基数が100の場合、a の配列は {56, 34, 12} と保存されていて、
aの最上位桁は a[s] = 12 となります。

1. [正規化] k := b / (b[t] + 1) を行う。a を k 倍したものを a とし、b を k 倍したものを b とする。
2. [商の桁位置] もし a[s] < b[t] なら u := s - t - 1 そうでなければ u := s - t とする。
3. [q'の計算] q' := (a[s] R + a[s-1]) / b[t] を行い、r' := (a[s] R + a[s-1]) mod b[t] とする。
    q' = R または q' b[t-1] > R r' + a[s-2] かどうかテストする。
    そうなら、q' から1を引き、r' に b[t] を加え、r' < b ならこのテストを繰り返す
    （このテストで仮商 q' が1だけ大きすぎる場合のほとんどを高速に見分けることができ、
    また q' が2だけ大きすぎる場合の全てを排除することができる）。
4. [乗算と減算] a := a - b q' R^u とする。
5. [剰余のテスト] q[u] = q' を行う。手順4の結果が負なら手順6に、そうでなければ手順7に進む。
6. [足し戻す] q[u] から1を引き、a := a + b R^u とする
    （このステップが必要となる確率は非常に小さく、2/Rのオーダーにすぎない）。
7. [ループ] もし u != 0 なら手順2に戻る。
8. [非正規化] ここで q が求める商である。a を k で割れば剰余を得る。

実際には、ループの1回目のときだけ、手順2で求めた商の桁の位置を1増やしてから (u := u + 1)
計算しないと答えが合わない場合があります。繰り返しになりますが、上の手順は純正の Kunth's Algorithm D
ではないので悪しからず（商の桁位置 u の決め方以外は同じです）。

これまでのKunth's Algorithm Dの話をRubyで表すと次のようになります。
ここでは与えられた整数 a, b を基数の桁数で分割するという作業を行うので、
コードが間延びして見えますが実際の多倍長では既に基数で分割されていて且つ配列で保存されているので、
実装する時はこの作業は端折ります。

```ruby
def divmod_kunthD(_a, _b, radix:)
  a, b = _a, _b

  # a,bを基数の桁数で分割する
  radix_len = Math.log10(radix).to_i
  as = a.to_s.reverse.scan(/.{1,#{radix_len}}/).map(&:reverse).map(&:to_i)
  bs = b.to_s.reverse.scan(/.{1,#{radix_len}}/).map(&:reverse).map(&:to_i)
  qs = [] # ans
  s = as.length - 1
  t = bs.length - 1

  k = radix / (bs[t] + 1)
  a *= k
  b *= k

  isFirst = 1
  while true
    # a,bを基数の桁数で分割する
    as = a.to_s.reverse.scan(/.{1,#{radix_len}}/).map(&:reverse).map(&:to_i)
    bs = b.to_s.reverse.scan(/.{1,#{radix_len}}/).map(&:reverse).map(&:to_i)
    s = as.length - 1 + isFirst
    t = bs.length - 1
    as.append(0)
    u = (as[s] < bs[t]) ? s - t - 1 : s - t;
    qh, rh = (as[s] * radix + as[s-1]).divmod( bs[t] )

    while rh < radix && qh * bs[t-1] > radix * rh + as[s-2]
      qh -= 1
      rh += bs[t]
    end

    qs[u] = qh

    a -= b * qh * radix**u
    if a < 0
      qs[u] -= 1
      a += b * radix**u
    end

    return [qs.reverse, a / k] if u <= 0
    isFirst = 0
  end
end

a = 109739200_000000000_000000000_000000000
b = 104876113_635753874
res = divmod_kunthD(a, b, radix: 1000000000)
puts "result   = #{res}"
puts "expected = #{a.divmod(b)}"
```


ちなみに Kunth's Algorithm D は Divide の「D」に由来します。
ついでに参考文献も載せておきますので参考までに。

- Kunth, D.E., 斎藤博昭・長尾高広・松井翔悟・松井孝雄・山内斉訳:
The Art of Computer Programming Volume 2 Seminumerical Algorithms Third Edition 日本語版
- 山本ワールド：多倍長整数の除算
http://yamatyuu.net/computer/program/long/div/index.html


### ニュートンラプソン法で平方根

課題によっては平方根を求める必要があるので、ニュートンラプソン法を使うことになると思います。
√N を求める反復式は x' = 1/2 (x + N/x) なので、Nを求めたい桁数dだけ 10^d を掛ければ √N を求めることができます。
しかし、反復式の N/x で除算が発生するために全体の処理が遅くなります。

一方、1/√N を求める反復式は x' = x/2 (3 - N x^2) なので、除算が発生しません。
そこで、1/√N を求めてからそれに N を掛けることで √N を求めることにします。
しかし、実際に計算しようとすると x^3 が行われるため、1サイクルの内に桁数が3倍になります。
そこで、x * x で桁が2倍になったタイミングで下半分の桁数を捨てる作業を行います。
この話を Ruby のプログラムに書き下したのが次のコードになります。
ニュートンラプソン法の x の初期値の決め方はテキストですでに議論されているのでそちらを参照してください。

```ruby
# Calculate 1/sqrt(n)
def inverse_sqrt(n, prec:)
  f = Proc.new do |x|
    nx2 = (n * x**2) / 10**prec  # Nx^2 を求めたのち下半分の桁を捨てる
    (x * (3 * 10**prec - nx2) / 2) / 10**prec  # 同様に下半分の桁を捨てる
  end

  n_length = Math.log10(n).ceil
  x = 2 * 10**(prec - n_length - 1)
  for i in 1..100
    puts "%2d) #{x}" % i
    x_next = f.(x)
    break if x_next - x == 0
    x = x_next
  end
  x
end

# √3 = 3 * 1/√3
sqrt3 = 3 * inverse_sqrt(3, prec: 30)
```

この「乗算をして桁が2倍になったときに下半分の桁数を捨てる技」は
ニュートンラプソン法で逆数を計算するときにも同様に使えます。


### ホーナー法（Horner法）

計算するときに少し工夫をする（ホーナー法を使う）と乗算の回数が減ります。
ホーナー法を使った式と使わない式を次に示します。

1. f(x) = a3 x^3 + a2 x^2 + a1 x + a0
2. f(x) = x (x (a3 x + a2) + a1) + a0

1の場合では 6 回掛け算が発生しますが、2の場合では掛け算は 3 回しか行わないため、
掛け算の回数を減らすことができました。


### 通分

あらかじめ分数を通分しておくことで、割り算の回数を減らすことができます。
下に示す式は、1は元の関数で、2は通分をした関数、3はホーナー法を適用した関数です。

1. f(x) = 1/(2 x + 1) + 1/(3 x + 2) + 1/(4 x + 3)
2. f(x) = (26 x^2 + 34 x + 11) / ((2 x + 1) (3 x + 2) (4 x + 3))
3. f(x) = (x (26 x + 34) + 11) / (x (x (24 x + 46) + 29) + 6)

1の場合では掛け算が3回、割り算が3回発生します。一方で
3の場合では掛け算が5回、割り算が1回発生するため、割り算の回数を抑えることができました。

余談ですが、式変形が面倒だと感じる場合は
<a href="https://www.wolframalpha.com/input/?i=1%2F(2+x+%2B+1)+%2B+1%2F(3+x+%2B+2)+%2B+1%2F(4+x+%2B+3)">
  WolframAlpha: 1/(2 x + 1) + 1/(3 x + 2) + 1/(4 x + 3)
</a>
のようにWolframAlphaに式をぶち込んで Alternate form のところを参考にすると作業が捗ります。


### その他

昨今の多倍長演算では、乗算はFFT（高速フーリエ変換）を用いた方法、
除算はニュートンラプソン法で除数の逆数を求めてから被除数に掛ける方法が広く用いられているようです。


## あとがき

私が3Jのときに、I先生はある生徒の話をしました。
その生徒は多倍長演算を高速化してないが故に円周率1000桁を求めるだけで数時間を要して大変苦労したので、
皆さんも計画的に課題に取り組みましょう、といった内容でした。
それを聞いて意地でも高速化して計算を一瞬で終わらせてやると闘志を燃やしていました。
実際、ネイピア数の小数点以下1000桁を求めるという課題を1秒未満で計算することに成功しました。
内心では一矢報いることができたと高を括っていたわけです（おそらくJ科歴代最速）。

時は経ち本科卒業目前、資源ごみとして捨てられていた多倍長演算のレポートを読んでみたところ、多倍長演算の高速化欲が再燃。
2016年1月に出された全課題（N = 0..10）を解きつつ、今まで作ったプログラム群を改良していきました。
3Jの時は Kunth先生の除算アルゴリズムを実装していなかったので、それを実装して処理速度が10倍くらい速くなりました。
私事ですが、約1万円で買った本「The Art of Computer Programming Seminumerical Algorithms 日本語訳」
が4年の時を得てやっと役に立てることができたことが個人的には嬉しい限りです。

長野高専の3Jで多倍長演算を学ぶ学生にこのレポジトリが目に留まれば幸いです。Happy Coding!

(set-logic QF_S)

(declare-fun a () String)
(declare-fun b () String)


(assert (= (concat a "bbba" ) (concat b "bbbba")))

(check-sat)

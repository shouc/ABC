(set-logic QF_S)

(declare-fun var_abc () String)

(assert (= (lastIndexOf var_abc /b(cb)*a+/) 2))

(check-sat)


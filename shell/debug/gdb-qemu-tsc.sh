b kvm_update_clock
c
set $v=first_cpu
while ($v)
p $v->tsc
set $v=$v->next_cpu
end


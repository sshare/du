package com.example.nettest;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.widget.EditText;
import android.widget.RadioButton;
import android.widget.RadioGroup;

import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;

import com.example.nettest.fragment.Fragment1;
import com.example.nettest.fragment.Fragment2;
import com.example.nettest.fragment.Fragment3;

public class HomeActivity extends AppCompatActivity  implements RadioGroup.OnCheckedChangeListener {
    private RadioGroup rg_tab_bar;
    private RadioButton rb_1;
    private RadioButton rb_2;
    private RadioButton rb_3;

    //Fragment Object
    private Fragment1 fg1;
    private Fragment2 fg2;
    private Fragment3 fg3;
    private FragmentManager fManager;
//    private Toolbar toolbar;DC2010016615FAAC
    private EditText devID_et,pwd_et;//编辑设备ID与密码
    public static  String deviceID,pwd;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_home);
        fManager = getSupportFragmentManager();
        pwd_et = findViewById(R.id.pwd_et);
        devID_et = findViewById(R.id.devID_et);

        pwd_et.addTextChangedListener( new EditTextChangedListener(pwd_et.getId()));
        devID_et.addTextChangedListener(new EditTextChangedListener(devID_et.getId()));

        rg_tab_bar = (RadioGroup) findViewById(R.id.rg_tab_bar);
        rg_tab_bar.setOnCheckedChangeListener(this);
        //获取第一个单选按钮，并设置其为选中状态
        rb_1 = (RadioButton) findViewById(R.id.rbb_1);
        rb_2 = (RadioButton) findViewById(R.id.rbb_2);
        rb_3 = (RadioButton) findViewById(R.id.rbb_3);
        rb_1.setChecked(true);
        initView();
    }

    @SuppressLint("ResourceAsColor")
    @Override
    public void onCheckedChanged(RadioGroup group, int checkedId) {
        FragmentTransaction fTransaction = fManager.beginTransaction();
        hideAllFragment(fTransaction);
        rb_1.setBackgroundColor(getResources().getColor(R.color.colorHint));
        rb_2.setBackgroundColor(getResources().getColor(R.color.colorHint));
        rb_3.setBackgroundColor(getResources().getColor(R.color.colorHint));
        Log.e("123123","111111111111");
        switch (checkedId){
            case R.id.rbb_1:
                rb_1.setBackgroundColor(0xff789789);
                if(fg1 == null){
                    fg1 = new Fragment1();
                    fTransaction.add(R.id.ly_content,fg1, "Fragment1");
                }else{
                    fTransaction.show(fg1);
                }
                break;
            case R.id.rbb_2:
                Log.e("123123","2");
                rb_2.setBackgroundColor(0xff789789);
                if(fg2 == null){
                    fg2 = new Fragment2();
                    fTransaction.add(R.id.ly_content,fg2, "Fragment2");
                }else{
                    fTransaction.show(fg2);
                }
                break;
            case R.id.rbb_3:
                Log.e("123123","3");
                rb_3.setBackgroundColor(0xff789789);
                if(fg3 == null){
                    fg3 = new Fragment3();
                    fTransaction.add(R.id.ly_content,fg3, "Fragment3");
                }else{
                    fTransaction.show(fg3);
                }
                break;
        }
        fTransaction.commit();
    }

    //隐藏所有Fragment
    private void hideAllFragment(FragmentTransaction fragmentTransaction){
        if(fg1 != null)fragmentTransaction.hide(fg1);
        if(fg2 != null)fragmentTransaction.hide(fg2);
//        if(fg3 != null)fragmentTransaction.hide(fg3);
        if(fg3 != null)fragmentTransaction.hide(fg3);
    }

    private void initView() {

    }

    class EditTextChangedListener implements TextWatcher {
        int editID;
        public EditTextChangedListener(int editID) {
            this.editID = editID;
//            this.editText = editText;
        }
        @Override
        public void beforeTextChanged(CharSequence s, int start, int count, int after) {

        }
        @Override
        public void onTextChanged(CharSequence s, int start, int before, int count) {

        }
        @Override
        public void afterTextChanged(Editable s) {
            switch (editID){
                case R.id.devID_et://设备ID
                    deviceID=s.toString();
                    break;
                case R.id.pwd_et://密码
                    pwd=s.toString();
                    break;
            }
        }
    }

}

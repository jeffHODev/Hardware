package com.hekangyuan.nivi1000.activity;

import android.text.InputType;
import android.text.Selection;
import android.text.Spannable;
import android.text.method.HideReturnsTransformationMethod;
import android.text.method.PasswordTransformationMethod;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.hekangyuan.nivi1000.R;
import com.hekangyuan.nivi1000.activity.api.LoginView;
import com.hekangyuan.nivi1000.activity.api.PasswordView;
import com.hekangyuan.nivi1000.activity.presenter.LoginPresenter;
import com.hekangyuan.nivi1000.activity.presenter.ModifyPasswordPresenter;
import com.hekangyuan.nivi1000.base.BaseActivity;
import com.hekangyuan.nivi1000.base.mvp.BaseJsonMsg;
import com.hekangyuan.nivi1000.model.LoginBean;
import com.hekangyuan.nivi1000.model.PasswordBean;
import com.hekangyuan.nivi1000.utils.DeviceUtil;
import com.hekangyuan.nivi1000.utils.ToastUtils;


public class ModifyPasswordActivity extends BaseActivity<ModifyPasswordPresenter> implements PasswordView, View.OnClickListener {


    private EditText oldPassEdit;
    private EditText newPassEdit;
    private EditText newPassAgainEdit;
    private Button submitBtn, close;
    private RelativeLayout ispas1, ispas2;
    protected boolean isHidden = true;


    private PasswordBean passwordBean;

    @Override
    protected ModifyPasswordPresenter createPresenter() {
        return new ModifyPasswordPresenter(this);
    }

    @Override
    protected int getLayoutId() {
        return R.layout.activity_modify_password;
    }


    @Override
    public void onErrorCode(BaseJsonMsg model) {
        ToastUtils.showToast(ModifyPasswordActivity.this,model.getHeader().getErrmsg());
    }

    @Override
    public void showError(String msg) {
        super.showError(msg);
    }


    @Override
    protected void initData() {
        findViews();
        setClickListener();
    }

    private void setClickListener() {
        submitBtn.setOnClickListener(this);
        close.setOnClickListener(this);
        ispas1.setOnClickListener(this);
        ispas2.setOnClickListener(this);
    }


    private void findViews() {
        submitBtn = findViewById(R.id.submit);
        close = findViewById(R.id.close);
        ispas1 = findViewById(R.id.ispas1);
        ispas2 = findViewById(R.id.ispas2);

        oldPassEdit = findViewById(R.id.old_pass);
        newPassEdit = findViewById(R.id.new_pass);
        newPassAgainEdit = findViewById(R.id.new_pass_agin);
    }

    @Override
    public void onMainSuccess(BaseJsonMsg<PasswordBean> o) {
        if(o.getHeader().getErrcode() == 2){
            showToast(o.getHeader().getErrmsg());
            return;
        }
        passwordBean = o.getData();

        ToastUtils.showToast(ModifyPasswordActivity.this,"修改成功");
        finish();
    }


    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.submit:
                if(newPassEdit.getText().toString().trim().length() == 0 || newPassAgainEdit.getText().toString().trim().length() == 0){
                    ToastUtils.showToast(ModifyPasswordActivity.this,"两次新密码都需要填写");
                    break;
                }
                if(!check()){
                    ToastUtils.showToast(ModifyPasswordActivity.this,"两次密码不一致，请重新输入");
                    break;
                }
                modifyPass();
                break;
            case R.id.close:
                ModifyPasswordActivity.this.finish();
                break;
            case R.id.ispas1:
                showPwd(newPassEdit);
                break;
            case R.id.ispas2:
                showPwd(newPassAgainEdit);
                break;
        }
    }

    protected void showPwd(EditText editText) {
        if (isHidden) {
            //editText可见
            editText.setTransformationMethod(HideReturnsTransformationMethod.getInstance());
            /**图标改变*/

        } else {
            //editText不可见
            editText.setTransformationMethod(PasswordTransformationMethod.getInstance());
            /**图标改变*/

        }
        isHidden = !isHidden;
        editText.postInvalidate();
        //切换后将EditText光标置于末尾
        CharSequence charSequence = editText.getText();
        if (charSequence instanceof Spannable) {
            Spannable spanText = (Spannable) charSequence;
            Selection.setSelection(spanText, charSequence.length());
        }
    }

    private boolean check(){
        if(newPassEdit.getText().toString() != "" && newPassAgainEdit.getText().toString() != ""
            && newPassEdit.getText().toString().equals(newPassAgainEdit.getText().toString())){
            return true;
        }
        return false;
    }

    private PasswordBean initPasswordBean() {
        PasswordBean passwordBean = new PasswordBean();
        passwordBean.setDeviceNo(DeviceUtil.getSERIAL());
        passwordBean.setOldPass((oldPassEdit.getText()+"").trim());
        passwordBean.setNewPass((newPassEdit.getText()+"").trim());
        return passwordBean;
    }

    private void modifyPass() {
        PasswordBean passwordBean = initPasswordBean();
        mPresenter.modifyPassHttp(passwordBean);
    }

}

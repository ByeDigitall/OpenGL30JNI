package com.ideacarry.example20.peephole;

import android.content.Context;
import android.opengl.GLES30;

import com.ideacarry.stable.filter.BaseFilter;
import com.ideacarry.utils.CommonUtils;
import com.ideacarry.utils.GLShaderProgram;
import com.ideacarry.utils.TextureManager;

public class PeepholeFilter extends BaseFilter {

    private GLShaderProgram mProgram;

    @Override
    public void onCreate(Context context) {
        mProgram = new GLShaderProgram(new String(CommonUtils.readAssetFile(context, "example20/peephole/vs_peephole.glsl")),
                new String(CommonUtils.readAssetFile(context, "example20/peephole/fs_peephole.glsl")));

        mProgram.use();
        mProgram.setInt("s_texColor", 0);
        mProgram.setFloat("u_radius", 0.5f);
        mProgram.setFloat("u_scale", 0.2f);
    }

    @Override
    public void onSizeChange(Context context, int viewWidth, int viewHeight, int cameraWidth, int cameraHeight, int degree, int flip) {
        if (mProgram != null) {
            mProgram.use();
            mProgram.setFloat("u_ratio", (float) viewHeight / (float) viewWidth);
        }
    }

    @Override
    public TextureManager.RendererData onDraw(TextureManager.RendererData data, int commonVao) {
        TextureManager.RendererData src = mTextureManager.getData(data.width, data.height, true);
        GLES30.glBindFramebuffer(GLES30.GL_FRAMEBUFFER, src.framebuffer);

        GLES30.glClearColor(0.1f, 1.0f, 0.1f, 1.0f);
        GLES30.glClear(GLES30.GL_COLOR_BUFFER_BIT);

        mProgram.use();
        GLES30.glActiveTexture(GLES30.GL_TEXTURE0);
        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, data.texture);
        GLES30.glBindVertexArray(commonVao);
        GLES30.glDrawArrays(GLES30.GL_TRIANGLE_STRIP, 0, 4);

        //clear
        GLES30.glBindVertexArray(0);
        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, 0);
        GLES30.glUseProgram(0);
        mTextureManager.release(data);

        return src;
    }

    @Override
    public void onDestroy(Context context) {
        if (mProgram != null) {
            mProgram.release();
            mProgram = null;
        }
    }
}

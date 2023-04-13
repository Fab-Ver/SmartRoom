import {CButton, CCard, CCardBody, CCardFooter, CCardHeader, CContainer, CFormRange, CFormSwitch} from '@coreui/react'
import '@coreui/coreui/dist/css/coreui.min.css'
import './App.css'
import { useState } from 'react'

const App = () => {
  const [rollerBlind, setRollerBlind] = useState(0)
  return (
    <div className="center">
		<CCard>
			<CCardHeader><h2>Manage Room Status</h2></CCardHeader>
			<CCardBody>
				<CFormRange
        			label={'Roller Blind Level\t' + rollerBlind}
        			min={0}
        			max={100}
        			step={1}
					value={rollerBlind}
					onChange={(event) => setRollerBlind(event.target.value)}
    			></CFormRange>
				<CFormSwitch
					size='xl'
					label='Light'
      			></CFormSwitch>
			</CCardBody>
			<CCardFooter className='justify-content-end'>
					<CButton
						color='primary'
						shape='rounded'
						size='lg'
						type='button'
					>
						Save
					</CButton>
			</CCardFooter>
		</CCard>
      	
    </div>
  );
}

export default App;
